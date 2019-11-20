"""
Detailed description.
"""
import os
from PySide2 import QtCore as qtc
from PySide2 import QtGui as qtg
from PySide2 import QtWidgets as qtw
from . import settings
from . import gui_util
from . import gui_edit








class Text(qtw.QDialog):
    """
    Detailed description.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, text="", parent=None, speller=False):
        """
        Detailed description.

        text : Detailed description.

        parent : Detailed description.

        speller : Detailed description.
        """
        qtw.QDialog.__init__(self,parent)
        self.__speller = speller
        self.__text_edit = gui_edit.Plain_Text(text,self,speller=speller,popup=False)
        self.__speller_box = gui_util.Spell_Checker("Spell Check",settings.DICTIONARY)
        self.__spell_button = qtw.QPushButton("Spell Check",self)
        self.__setup_gui_()


    ####################
    # PUBLIC - Methods #
    ####################


    def text(self):
        """
        Detailed description.

        return : Yes
        """
        return self.__text_edit.toPlainText()


    #####################
    # PRIVATE - Methods #
    #####################


    def __setup_gui_(self):
        """
        Detailed description.
        """
        layout = qtw.QVBoxLayout()
        layout.addWidget(self.__text_edit)
        layout.addWidget(self.__setup_spell_checker_())
        layout.addLayout(self.__setup_buttons_())
        self.setLayout(layout)


    def __setup_spell_checker_(self):
        """
        Detailed description.

        return : Yes
        """
        self.__speller_box.hide()
        self.__speller_box.cursor_changed.connect(lambda cursor : self.__text_edit.setTextCursor(cursor))
        self.__speller_box.finished.connect(self.__spell_check_fnished_)
        return self.__speller_box


    def __setup_buttons_(self):
        """
        Detailed description.

        return : Yes
        """
        set_ = qtw.QPushButton("Set")
        set_.clicked.connect(lambda : self.done(qtw.QDialog.Accepted))
        cancel = qtw.QPushButton("Cancel")
        cancel.clicked.connect(lambda : self.done(qtw.QDialog.Rejected))
        self.__spell_button.clicked.connect(self.__spell_check_)
        self.__spell_button.setDisabled(not self.__speller)
        ret = qtw.QHBoxLayout()
        ret.addWidget(set_)
        ret.addWidget(cancel)
        ret.addStretch()
        ret.addWidget(self.__spell_button)
        return ret


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot()
    def __spell_check_(self):
        """
        Detailed description.
        """
        self.__speller_box.show()
        self.__speller_box.start(self.__text_edit.textCursor())


    @qtc.Slot()
    def __spell_check_fnished_(self):
        """
        Detailed description.
        """
        self.__speller_box.hide()
        cursor = self.__text_edit.textCursor()
        cursor.clearSelection()
        self.__text_edit.setTextCursor(cursor)
