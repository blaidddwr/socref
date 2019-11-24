"""
Detailed description.
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
    Detailed description.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, dictionary, parent):
        """
        Detailed description.

        dictionary : Detailed description.

        parent : Detailed description.
        """
        qtg.QSyntaxHighlighter.__init__(self,parent)
        self.__hunspell = hunspell.HunSpell(os.path.join(settings.HUNSPELL_ROOT,dictionary + ".dic")
                                            ,os.path.join(settings.HUNSPELL_ROOT,dictionary + ".aff"))
        format_ = self.__format = qtg.QTextCharFormat()
        format_.setFontUnderline(True)
        format_.setUnderlineColor(qtc.Qt.red)
        format_.setUnderlineStyle(qtg.QTextCharFormat.WaveUnderline)


    ####################
    # PUBLIC - Methods #
    ####################


    def highlightBlock(self, text):
        """
        Detailed description.

        text : Detailed description.
        """
        start = 0
        pattern = re.compile('\w+')
        while True :
            match = pattern.search(text,start)
            if not match : break
            if not self.__hunspell.spell(match.group(0)) :
                self.setFormat(match.start(),match.end() - match.start(),self.__format)
            start = match.end()








class Spell_Checker(qtw.QGroupBox):
    """
    Detailed description.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, title, dictionary, parent=None):
        """
        Detailed description.

        title : Detailed description.

        dictionary : Detailed description.

        parent : Detailed description.
        """
        qtw.QGroupBox.__init__(self,title,parent)
        self.__hunspell = hunspell.HunSpell(os.path.join(settings.HUNSPELL_ROOT,dictionary + ".dic")
                                            ,os.path.join(settings.HUNSPELL_ROOT,dictionary + ".aff"))
        self.__word_label = qtw.QLabel(self)
        self.__word_edit = qtw.QLineEdit(self)
        self.__cursor = None
        self.__suggested = []
        self.__setup_gui_()


    ####################
    # PUBLIC - Signals #
    ####################


    #
    # Detailed description.
    #
    finished = qtc.Signal()
    #
    # Detailed description.
    #
    cursor_changed = qtc.Signal(qtg.QTextCursor)


    ##################
    # PUBLIC - Slots #
    ##################


    @qtc.Slot(qtg.QTextCursor)
    def start(self, cursor):
        """
        Detailed description.

        cursor : Detailed description.
        """
        if self.__cursor is None :
            self.__cursor = cursor
            cursor.movePosition(qtg.QTextCursor.Start)
            self.__find_next_word_()


    #####################
    # PRIVATE - Methods #
    #####################


    def __find_next_word_(self):
        """
        Detailed description.
        """
        cursor = self.__cursor
        while True :
            cursor.select(qtg.QTextCursor.WordUnderCursor)
            if cursor.selectedText() :
                hun = self.__hunspell
                word = cursor.selectedText()
                if not hun.spell(word) :
                    self.cursor_changed.emit(cursor)
                    self.__word_label.setText(word)
                    self.__word_edit.setText(word)
                    self.__suggested = hun.suggest(word)
                    break
            if not cursor.movePosition(qtg.QTextCursor.NextWord) :
                self.__stop_()
                break


    def __setup_gui_(self):
        """
        Detailed description.
        """
        layout = qtw.QVBoxLayout()
        layout.addLayout(self.__setup_top_())
        layout.addLayout(self.__setup_bottom_())
        self.setLayout(layout)


    def __setup_top_(self):
        """
        Detailed description.

        return : Yes
        """
        add = qtw.QPushButton("<< Add to Dictionary")
        add.clicked.connect(self.__add_)
        ret = qtw.QHBoxLayout()
        ret.addWidget(self.__word_label)
        ret.addStretch()
        ret.addWidget(add)
        return ret


    def __setup_bottom_(self):
        """
        Detailed description.

        return : Yes
        """
        suggest = qtw.QPushButton("Suggest")
        suggest.clicked.connect(self.__suggest_)
        replace = qtw.QPushButton("Replace")
        replace.clicked.connect(self.__replace_)
        ignore = qtw.QPushButton("Ignore")
        ignore.clicked.connect(self.__ignore_)
        stop = qtw.QPushButton("Stop")
        stop.clicked.connect(self.__stop_)
        ret = qtw.QHBoxLayout()
        ret.addWidget(self.__word_edit)
        ret.addStretch()
        ret.addWidget(suggest)
        ret.addWidget(replace)
        ret.addWidget(ignore)
        ret.addWidget(stop)
        return ret


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot()
    def __add_(self):
        """
        Detailed description.
        """
        if self.__cursor is not None:
            self.__hunspell.add(self.__word_edit.text())
            if not self.__cursor.movePosition(qtg.QTextCursor.NextWord) : self.__stop_()
            else: self.__find_next_word_()


    @qtc.Slot()
    def __suggest_(self):
        """
        Detailed description.
        """
        if self.__cursor is not None:
            index = 1
            try:
                index += self.__suggested.index(self.__word_edit.text())
            except ValueError:
                index = 0
            if index >= len(self.__suggested) : index = 0
            self.__word_edit.setText(self.__suggested[index])


    @qtc.Slot()
    def __replace_(self):
        """
        Detailed description.
        """
        if self.__cursor is not None:
            self.__cursor.insertText(self.__word_edit.text())
            if not self.__cursor.movePosition(qtg.QTextCursor.NextWord) : self.__stop_()
            else: self.__find_next_word_()


    @qtc.Slot()
    def __ignore_(self):
        """
        Detailed description.
        """
        if self.__cursor is not None :
            if not self.__cursor.movePosition(qtg.QTextCursor.NextWord) : self.__stop_()
            else: self.__find_next_word_()


    @qtc.Slot()
    def __stop_(self):
        """
        Detailed description.
        """
        if self.__cursor is not None :
            self.__cursor = None
            self.__suggested = []
            self.finished.emit()
