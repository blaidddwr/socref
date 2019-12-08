"""
Contains all GUI utility classes used by other GUI elements of the core application.
"""
import os
import re
from PySide2 import QtCore as qtc
from PySide2 import QtGui as qtg
from PySide2 import QtWidgets as qtw
import hunspell
from . import settings








class SpellHighlighter(qtg.QSyntaxHighlighter):
    """
    This is the spell highlighter class. It implements the qt syntax highlighter class. It provides
    highlighting of misspelled words in the document the highlighter is given on initialization. It
    uses the Hunspell library for all speller functions.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, dictionary, parent):
        """
        Initializes a new spell highlighter with the given dictionary and parent.

        dictionary : The Hunspell dictionary used for spell checking. This does not include the
                     directory or file extensions, for example "en_US".

        parent : The parent document that takes ownership of this highlighter and has its misspelled
                 words highlighted.
        """
        qtg.QSyntaxHighlighter.__init__(self,parent)
        self.__hunspell = hunspell.HunSpell(
            os.path.join(settings.HUNSPELL_ROOT,dictionary + ".dic")
            ,os.path.join(settings.HUNSPELL_ROOT,dictionary + ".aff")
        )
        format_ = self.__format = qtg.QTextCharFormat()
        format_.setFontUnderline(True)
        format_.setUnderlineColor(qtc.Qt.red)
        format_.setUnderlineStyle(qtg.QTextCharFormat.WaveUnderline)


    ####################
    # PUBLIC - Methods #
    ####################


    def highlightBlock(self, text):
        """
        Implements the qtg.QSyntaxHighlighter interface.

        text : See qt docs.
        """
        start = 0
        pattern = re.compile('\w+')
        while True:
            match = pattern.search(text,start)
            if not match:
                break
            if not self.__hunspell.spell(match.group(0)):
                self.setFormat(match.start(),match.end() - match.start(),self.__format)
            start = match.end()








class SpellChecker(qtw.QGroupBox):
    """
    This is the spell checker class. It provides a spell checking and correction interface. It uses
    the Hunspell library for all speller functions.

    It provides a spell checking and correction interface. It begins spell checking with its start
    slot, providing it a qt text cursor that is uses to parse the entire document of the text
    cursor, checking for misspelled words. Each misspelled word is highlighted and waits on the user
    to correct. The user is given options to use a suggestion, replace, ignore, stop, or add to
    dictionary. The cursor is updated to highlight each misspelled word found while waiting for the
    user to make a choice about it. Adding to dictionary is currently defunct but will be fixed.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, title, dictionary, parent=None):
        """
        Initializes a new speller checker with the given title, dictionary, and optional parent.

        title : The title of this new spell checker's qt group box.

        dictionary : The Hunspell dictionary used for spell checking. This does not include the
                     directory or file extensions, for example "en_US".

        parent : An optional qt object parent for this new spell checker.
        """
        qtw.QGroupBox.__init__(self,title,parent)
        self.__hunspell = hunspell.HunSpell(
            os.path.join(settings.HUNSPELL_ROOT,dictionary + ".dic")
            ,os.path.join(settings.HUNSPELL_ROOT,dictionary + ".aff")
        )
        self.__wordLabel = qtw.QLabel(self)
        self.__wordEdit = qtw.QLineEdit(self)
        self.__cursor = None
        self.__suggested = []
        self.__setupGui_()


    ####################
    # PUBLIC - Signals #
    ####################


    #
    # Signals this spell checker has changed the cursor given to it.
    #
    cursorChanged = qtc.Signal(qtg.QTextCursor)
    #
    # Signals this spell checker has finished checking and correcting for misspelled words.
    #
    finished = qtc.Signal()


    ##################
    # PUBLIC - Slots #
    ##################


    @qtc.Slot(qtg.QTextCursor)
    def start(self, cursor):
        """
        Called to have this spell checker begin checking for misspelled words in the document of the
        given qt text cursor. The entire document is checked for misspelled words. If this spell
        checker is already checking a document then this does nothing.

        cursor : A qt text cursor whose document is checked for misspelled words.
        """
        if self.__cursor is None:
            self.__cursor = cursor
            cursor.movePosition(qtg.QTextCursor.Start)
            self.__findNextWord_()


    #####################
    # PRIVATE - Methods #
    #####################


    def __findNextWord_(self, skip=False):
        """
        Finds the next misspelled word in the current document this spell checker is checking,
        optionally skipping the word this checker's cursor is currently under. If no misspelled word
        is found once the end of the document is reached then it stops this spell checker.

        skip : True to skip the word this checker's cursor is currently under or false to check it
               for spelling.
        """
        cursor = self.__cursor
        pattern = re.compile('\w+')
        while True:
            if not skip:
                cursor.select(qtg.QTextCursor.WordUnderCursor)
                if pattern.fullmatch(cursor.selectedText()):
                    hun = self.__hunspell
                    word = cursor.selectedText()
                    if not hun.spell(word):
                        self.cursorChanged.emit(cursor)
                        self.__wordLabel.setText(word)
                        self.__wordEdit.setText(word)
                        self.__suggested = hun.suggest(word)
                        break
            else:
                skip = False
            if not cursor.movePosition(qtg.QTextCursor.NextWord):
                self.__stop_()
                break


    def __setupGui_(self):
        """
        Initializes the GUI of this new spell checker.
        """
        suggest = qtw.QPushButton("Suggest")
        suggest.clicked.connect(self.__suggest_)
        replace = qtw.QPushButton("Replace")
        replace.clicked.connect(self.__replace_)
        ignore = qtw.QPushButton("Ignore")
        ignore.clicked.connect(self.__ignore_)
        ignore_all = qtw.QPushButton("Ignore All")
        ignore_all.clicked.connect(self.__ignoreAll_)
        stop = qtw.QPushButton("Stop")
        stop.clicked.connect(self.__stop_)
        layout = qtw.QHBoxLayout()
        layout.addWidget(self.__wordLabel)
        layout.addWidget(self.__wordEdit)
        layout.addStretch()
        layout.addWidget(suggest)
        layout.addWidget(replace)
        layout.addWidget(ignore)
        layout.addWidget(ignore_all)
        layout.addWidget(stop)
        self.setLayout(layout)


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot()
    def __ignore_(self):
        """
        Called to ignore the selected misspelled word, simply skipping it and moving the cursor
        forward looking for the next misspelled word. If this spell checker is not actively checking
        a document then this does nothing.
        """
        if self.__cursor is not None:
            self.__findNextWord_(skip=True)


    @qtc.Slot()
    def __ignoreAll_(self):
        """
        Called to ignore all words matching the currently selected misspelled word by adding it to
        the run time dictionary. This then moves the cursor forward looking for the next misspelled
        word. If this spell checker is not actively checking a document then this does nothing.
        """
        if self.__cursor is not None:
            self.__hunspell.add(self.__wordLabel.text())
            self.__findNextWord_(skip=True)


    @qtc.Slot()
    def __replace_(self):
        """
        Called to replace the selected misspelled word with the text of this spell checker's word
        edit widget. This then moves the cursor forward looking for the next misspelled word. If
        this spell checker is not actively checking a document then this does nothing.
        """
        if self.__cursor is not None:
            self.__cursor.insertText(self.__wordEdit.text())
            self.__findNextWord_()


    @qtc.Slot()
    def __stop_(self):
        """
        Called to stop this spell checker, finishing its spell checking of the current document. If
        this spell checker is not actively checking a document then this does nothing.
        """
        if self.__cursor is not None:
            self.__cursor = None
            self.__suggested = []
            self.finished.emit()


    @qtc.Slot()
    def __suggest_(self):
        """
        Called to offer a suggested word in this spell checker's word edit widget. This finds the
        next word in the list of suggestions. If this spell checker is not actively checking a
        document then this does nothing.
        """
        if self.__cursor is not None and self.__suggested:
            #
            # Find the index to the next word in this spell checker's list of suggestions, wrapping
            # to the beginning if its word edit widget is on the last word or an unknown word.
            #
            index = 1
            try:
                index += self.__suggested.index(self.__wordEdit.text())
            except ValueError:
                index = 0
            if index >= len(self.__suggested):
                index = 0
            self.__wordEdit.setText(self.__suggested[index])
