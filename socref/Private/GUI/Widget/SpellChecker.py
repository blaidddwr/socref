"""
Contains the SpellChecker class.
"""
from ...Model import speller
from re import compile as reCompile
from PySide6.QtCore import Signal
from PySide6.QtCore import Slot
from PySide6.QtGui import QTextCursor
from PySide6.QtWidgets import QGroupBox
from PySide6.QtWidgets import QHBoxLayout
from PySide6.QtWidgets import QLabel
from PySide6.QtWidgets import QLineEdit
from PySide6.QtWidgets import QPushButton




class SpellChecker(QGroupBox):
    """
    This is the spell checker class. It provides a spell checking and correction
    interface using the Hunspell library. It begins spell checking with its
    start slot, providing it a Qt text cursor that is used to parse the entire
    document of the text cursor, checking for misspelled words. Each misspelled
    word is highlighted and waits on the user to correct. The user is given
    options to use a suggestion, replace, ignore, or stop. The cursor is updated
    to highlight each misspelled word found while waiting for the user to make a
    choice about it.
    """
    __wordPattern = reCompile('\w+')


    #
    # Signals this spell checker has changed the cursor given to it.
    #
    cursorChanged = Signal(QTextCursor)


    #
    # Signals this spell checker has finished checking and correcting for
    # misspelled words.
    #
    finished = Signal()


    def __init__(
        self
        ,title
        ,parent=None
    ):
        """
        Initializes this new speller checker with the given group box title and
        optional Qt object parent.

        Parameters
        ----------
        title : string
                The group box title.
        parent : QObject
                 The optional Qt object parent.
        """
        super().__init__(title,parent)
        self.__wordLabel = QLabel(self)
        self.__wordEdit = QLineEdit(self)
        self.__cursor = None
        self.__suggested = []
        self.__setupGui_()


    @Slot(QTextCursor)
    def start(
        self
        ,cursor
    ):
        """
        Called to have this spell checker begin checking for misspelled words in
        the document of the given Qt text cursor. The entire document is checked
        for misspelled words. If this spell checker is already checking a
        document then this does nothing.

        Parameters
        ----------
        cursor : QTextCursor
                 The Qt text cursor.
        """
        if self.__cursor is None:
            self.__cursor = cursor
            cursor.movePosition(QTextCursor.Start)
            self.__findNextWord_()


    def __findNextWord_(
        self
        ,skip=False
    ):
        """
        Finds the next misspelled word in the current document of this spell
        checker's Qt text cursor. The given skip flag determines if the current
        word is ignored and skipped. The default is to not ignore the current
        word for misspelling. If no misspelled word is found once the end of the
        document is reached then this spell checker stops.

        Parameters
        ----------
        skip : bool
               True to skip the current word or false otherwise.
        """
        cursor = self.__cursor
        while True:
            if not skip:
                cursor.select(QTextCursor.WordUnderCursor)
                if self.__wordPattern.fullmatch(cursor.selectedText()):
                    word = cursor.selectedText()
                    if not speller.spell(word):
                        self.cursorChanged.emit(cursor)
                        self.__wordLabel.setText(word)
                        self.__wordEdit.setText(word)
                        self.__suggested = speller.suggest(word)
                        break
            else:
                skip = False
            if not cursor.movePosition(QTextCursor.NextWord):
                self.__stop_()
                break


    @Slot()
    def __ignore_(
        self
    ):
        """
        Called to ignore the selected misspelled word, simply skipping it and
        moving the cursor forward looking for the next misspelled word. If this
        spell checker is not actively checking a document then this does
        nothing.
        """
        if self.__cursor is not None:
            self.__findNextWord_(skip=True)


    @Slot()
    def __ignoreAll_(
        self
    ):
        """
        Called to ignore all words matching the currently selected misspelled
        word by adding it to the run time dictionary. This then moves the cursor
        forward looking for the next misspelled word. If this spell checker is
        not actively checking a document then this does nothing.
        """
        if self.__cursor is not None:
            speller.add(self.__wordLabel.text())
            self.__findNextWord_(skip=True)


    @Slot()
    def __replace_(
        self
    ):
        """
        Called to replace the selected misspelled word with the text of this
        spell checker's word edit widget. This then moves the cursor forward
        looking for the next misspelled word. If this spell checker is not
        actively checking a document then this does nothing.
        """
        if self.__cursor is not None:
            self.__cursor.insertText(self.__wordEdit.text())
            self.__findNextWord_()


    def __setupGui_(
        self
    ):
        """
        Initializes the GUI of this new spell checker.
        """
        suggest = QPushButton("Suggest")
        suggest.clicked.connect(self.__suggest_)
        replace = QPushButton("Replace")
        replace.clicked.connect(self.__replace_)
        ignore = QPushButton("Ignore")
        ignore.clicked.connect(self.__ignore_)
        ignore_all = QPushButton("Ignore All")
        ignore_all.clicked.connect(self.__ignoreAll_)
        stop = QPushButton("Stop")
        stop.clicked.connect(self.__stop_)
        layout = QHBoxLayout()
        layout.addWidget(self.__wordLabel)
        layout.addWidget(self.__wordEdit)
        layout.addStretch()
        layout.addWidget(suggest)
        layout.addWidget(replace)
        layout.addWidget(ignore)
        layout.addWidget(ignore_all)
        layout.addWidget(stop)
        self.setLayout(layout)


    @Slot()
    def __stop_(
        self
    ):
        """
        Called to stop this spell checker, finishing its spell checking of the
        current document. If this spell checker is not actively checking a
        document then this does nothing.
        """
        if self.__cursor is not None:
            self.__cursor = None
            self.__suggested = []
            self.finished.emit()


    @Slot()
    def __suggest_(
        self
    ):
        """
        Called to offer a suggested word in this spell checker's word edit
        widget. This finds the next word in the list of suggestions. If this
        spell checker is not actively checking a document then this does
        nothing.
        """
        if self.__cursor is not None and self.__suggested:
            index = 1
            try:
                index += self.__suggested.index(self.__wordEdit.text())
            except ValueError:
                index = 0
            if index >= len(self.__suggested):
                index = 0
            self.__wordEdit.setText(self.__suggested[index])
