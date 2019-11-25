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








class Spell_Highlighter(qtg.QSyntaxHighlighter):
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
        #
        # Initialize the Hunspell object with the given dictionary, appending the root path and
        # appropriate extensions.
        #
        self.__hunspell = hunspell.HunSpell(os.path.join(settings.HUNSPELL_ROOT,dictionary + ".dic")
                                            ,os.path.join(settings.HUNSPELL_ROOT,dictionary + ".aff"))
        #
        # Initialize the format style used for underlining misspelled words.
        #
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
        #
        # Initialize the search start and regular expression.
        #
        start = 0
        pattern = re.compile('\w+')
        #
        # Continue while there are still words to check.
        #
        while True :
            #
            # Attempt to get the next word matched, breaking out if there are no more words left.
            #
            match = pattern.search(text,start)
            if not match : break
            #
            # Check the words spelling, settings its format as misspelled if it is as such.
            #
            if not self.__hunspell.spell(match.group(0)) :
                self.setFormat(match.start(),match.end() - match.start(),self.__format)
            #
            # Update the search start to the end of the last word matched.
            #
            start = match.end()








class Spell_Checker(qtw.QGroupBox):
    """
    This is the spell checker class. It inherits the qt group box widget. It provides a spell
    checking and correction interface. It uses the Hunspell library for all speller functions.

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
        #
        # Initialize the Hunspell object with the given dictionary, appending the root path and
        # appropriate extensions.
        #
        self.__hunspell = hunspell.HunSpell(os.path.join(settings.HUNSPELL_ROOT,dictionary + ".dic")
                                            ,os.path.join(settings.HUNSPELL_ROOT,dictionary + ".aff"))
        #
        # Initialize the word label and edit widgets used for each misspelled word found.
        #
        self.__word_label = qtw.QLabel(self)
        self.__word_edit = qtw.QLineEdit(self)
        #
        # Initialize the cursor to none and list of suggested words to an empty list.
        #
        self.__cursor = None
        self.__suggested = []
        #
        # Setup this new widget's GUI.
        #
        self.__setup_gui_()


    ####################
    # PUBLIC - Signals #
    ####################


    #
    # Signals this spell checker has finished checking and correcting for misspelled words.
    #
    finished = qtc.Signal()
    #
    # Signals this spell checker has changed the cursor given to it.
    #
    cursor_changed = qtc.Signal(qtg.QTextCursor)


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
        #
        # Make sure this spell checker is not currently checking a document.
        #
        if self.__cursor is None :
            #
            # Set this spell checker's cursor to the one given and move it to the beginning of the
            # document.
            #
            self.__cursor = cursor
            cursor.movePosition(qtg.QTextCursor.Start)
            #
            # Find the next misspelled word.
            #
            self.__find_next_word_()


    #####################
    # PRIVATE - Methods #
    #####################


    def __find_next_word_(self):
        """
        Finds the next misspelled word in the current document this spell checker is checking. If no
        misspelled word is found once the end of the document is reached then it stops this spell
        checker.
        """
        #
        # Get this spell checker's cursor and compile the regular expression for matching words.
        #
        cursor = self.__cursor
        pattern = re.compile('\w+')
        #
        # Continue until a misspelled word is found or the cursor reaches the end of the document.
        #
        while True :
            #
            # Select the word under the cursor and make sure it is a word.
            #
            cursor.select(qtg.QTextCursor.WordUnderCursor)
            if pattern.fullmatch(cursor.selectedText()) :
                #
                # Get the speller and selected word.
                #
                hun = self.__hunspell
                word = cursor.selectedText()
                #
                # If the word is misspelled then signal the cursor has changed, update the word
                # label and edit widgets with the misspelled word, and update the list of
                # suggestions for the new misspelled word.
                #
                if not hun.spell(word) :
                    self.cursor_changed.emit(cursor)
                    self.__word_label.setText(word)
                    self.__word_edit.setText(word)
                    self.__suggested = hun.suggest(word)
                    break
            #
            # Move to the next word in the cursor's document, stopping this spell checker if the end
            # of the document is reached.
            #
            if not cursor.movePosition(qtg.QTextCursor.NextWord) :
                self.__stop_()
                break


    def __setup_gui_(self):
        """
        Initializes the GUI of this new spell checker.
        """
        layout = qtw.QVBoxLayout()
        layout.addLayout(self.__setup_top_())
        layout.addLayout(self.__setup_bottom_())
        self.setLayout(layout)


    def __setup_top_(self):
        """
        Initializes the top GUI of this new spell checker.

        return : Initialized top qt layout.
        """
        #
        # Initialize the add to dictionary button.
        #
        add = qtw.QPushButton("<< Add to Dictionary")
        add.clicked.connect(self.__add_)
        #
        # Create a horizontal box layout. Add this spell checker's word label, a stretch, and then
        # the add to dictionary button.
        #
        ret = qtw.QHBoxLayout()
        ret.addWidget(self.__word_label)
        ret.addStretch()
        ret.addWidget(add)
        #
        # Return the box layout.
        #
        return ret


    def __setup_bottom_(self):
        """
        Initializes the bottom GUI of this new spell checker.

        return : Initialized bottom qt layout.
        """
        #
        # Initialize the suggest, replace, ignore, and stop buttons.
        #
        suggest = qtw.QPushButton("Suggest")
        suggest.clicked.connect(self.__suggest_)
        replace = qtw.QPushButton("Replace")
        replace.clicked.connect(self.__replace_)
        ignore = qtw.QPushButton("Ignore")
        ignore.clicked.connect(self.__ignore_)
        stop = qtw.QPushButton("Stop")
        stop.clicked.connect(self.__stop_)
        #
        # Create a horizontal box layout. Add this spell checker's word edit, a stretch, suggest
        # button, replace button, ignore button, and then stop button.
        #
        ret = qtw.QHBoxLayout()
        ret.addWidget(self.__word_edit)
        ret.addStretch()
        ret.addWidget(suggest)
        ret.addWidget(replace)
        ret.addWidget(ignore)
        ret.addWidget(stop)
        #
        # Return the box layout.
        #
        return ret


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot()
    def __add_(self):
        """
        Called to add the currently selected misspelled word to the run time dictionary. This is
        currently defunct so TO BE DONE.
        """
        if self.__cursor is not None:
            self.__hunspell.add(self.__word_edit.text())
            if not self.__cursor.movePosition(qtg.QTextCursor.NextWord) : self.__stop_()
            else: self.__find_next_word_()


    @qtc.Slot()
    def __suggest_(self):
        """
        Called to offer a suggested word in this spell checker's word edit widget. This finds the
        next word in the list of suggestions. If this spell checker is not actively checking a
        document then this does nothing.
        """
        #
        # Make sure this spell checker is active.
        #
        if self.__cursor is not None:
            #
            # Find the index to the next word in this spell checker's list of suggestions, wrapping
            # to the beginning if its word edit widget is on the last word or an unknown word.
            #
            index = 1
            try:
                index += self.__suggested.index(self.__word_edit.text())
            except ValueError:
                index = 0
            if index >= len(self.__suggested) : index = 0
            #
            # Update the word edit widget with the next suggested word.
            #
            self.__word_edit.setText(self.__suggested[index])


    @qtc.Slot()
    def __replace_(self):
        """
        Called to replace the selected misspelled word with the text of this spell checker's word
        edit widget. This then moves the cursor forward looking for the next misspelled word. If
        this spell checker is not actively checking a document then this does nothing.
        """
        #
        # Make sure this spell checker is active.
        #
        if self.__cursor is not None:
            #
            # Replace the cursor's selected word with this spell checker's word edit value.
            #
            self.__cursor.insertText(self.__word_edit.text())
            #
            # Move the cursor to the next word, continuing or finishing if there is a word or not,
            # respectively.
            #
            if not self.__cursor.movePosition(qtg.QTextCursor.NextWord) : self.__stop_()
            else: self.__find_next_word_()


    @qtc.Slot()
    def __ignore_(self):
        """
        Called to ignore the selected misspelled word, simply skipping it and moving the cursor
        forward looking for the next misspelled word. If this spell checker is not actively checking
        a document then this does nothing.
        """
        #
        # Make sure this spell checker is active.
        #
        if self.__cursor is not None :
            #
            # Move the cursor to the next word, continuing or finishing if there is a word or not,
            # respectively.
            #
            if not self.__cursor.movePosition(qtg.QTextCursor.NextWord) : self.__stop_()
            else: self.__find_next_word_()


    @qtc.Slot()
    def __stop_(self):
        """
        Called to stop this spell checker, finishing its spell checking of the current document. If
        this spell checker is not actively checking a document then this does nothing.
        """
        #
        # Make sure this spell checker is active.
        #
        if self.__cursor is not None :
            #
            # Reset this spell checker's cursor to none and its list of suggested words to empty.
            #
            self.__cursor = None
            self.__suggested = []
            #
            # Signal that this spell checker has finished.
            #
            self.finished.emit()
