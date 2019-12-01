"""
Contains all GUI dialog windows used by the core application.
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
    This is the text dialog class. It inherits the qt dialog class. It is a plain text editor with
    optional spell checking and correction. Any misspelled words are highlighted and a button is
    provided that begins spell checking and correction. All spell checking features can be enabled
    or disabled at initialization of the dialog.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, text="", parent=None, speller=False):
        """
        Initializes a new text dialog with the given optional text and parent. Spelling features can
        also be enabled or disabled.

        text : The optional text of this new text dialog.

        parent : The optional qt object parent of this new text dialog.

        speller : True to enable misspelled word highlighting and spell checking or false to disable
                  it all.
        """
        qtw.QDialog.__init__(self,parent)
        #
        # Initialize the speller indicator.
        #
        self.__speller = speller
        #
        # Initialize this dialog's text edit widget, speller group box, and spell checking button.
        #
        self.__text_edit = gui_edit.Plain_Text(text,self,speller=speller,popup=False)
        self.__speller_box = gui_util.Spell_Checker("Spell Check",settings.DICTIONARY)
        self.__spell_button = qtw.QPushButton("Spell Check",self)
        #
        # Setup the GUI of this dialog.
        #
        self.__setup_gui_()


    ####################
    # PUBLIC - Methods #
    ####################


    def text(self):
        """
        Getter method.

        return : The current text of this text dialog.
        """
        return self.__text_edit.toPlainText()


    #####################
    # PRIVATE - Methods #
    #####################


    def __setup_gui_(self):
        """
        Initializes the GUI of this new text dialog.
        """
        #
        # Create a vertical box layout. Add this dialog's text edit widget, spell checker box, and
        # then its buttons.
        #
        layout = qtw.QVBoxLayout()
        layout.addWidget(self.__text_edit)
        layout.addWidget(self.__setup_spell_checker_())
        layout.addLayout(self.__setup_buttons_())
        #
        # Set this dialog's layout.
        #
        self.setLayout(layout)


    def __setup_spell_checker_(self):
        """
        Initializes the spell checker box of this new text dialog.

        return : The initialized spell checker box widget.
        """
        #
        # Initialize this dialog's spell checker box.
        #
        self.__speller_box.hide()
        self.__speller_box.cursor_changed.connect(lambda cursor : self.__text_edit.setTextCursor(cursor))
        self.__speller_box.finished.connect(self.__spell_check_finished_)
        #
        # Return the spell checker box.
        #
        return self.__speller_box


    def __setup_buttons_(self):
        """
        Initializes the buttons of this new text dialog.

        return : A box layout of initialized buttons.
        """
        #
        # Initialize this dialog's set and cancel buttons.
        #
        set_ = qtw.QPushButton("Set")
        set_.clicked.connect(lambda : self.done(qtw.QDialog.Accepted))
        cancel = qtw.QPushButton("Cancel")
        cancel.clicked.connect(lambda : self.done(qtw.QDialog.Rejected))
        #
        # Initialize this dialog's spell check button, settings is enabled state if spelling
        # features are on.
        #
        self.__spell_button.clicked.connect(self.__spell_check_)
        self.__spell_button.setDisabled(not self.__speller)
        #
        # Create a horizontal box layout. Add the set button, cancel button, a stretch, and then the
        # spell check button.
        #
        ret = qtw.QHBoxLayout()
        ret.addWidget(set_)
        ret.addWidget(cancel)
        ret.addStretch()
        ret.addWidget(self.__spell_button)
        #
        # Return the box layout.
        #
        return ret


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot()
    def __spell_check_(self):
        """
        Called to begin spell checking this text dialog's document. This shows its spell checker box
        and tells it to start checking.
        """
        self.__speller_box.show()
        self.__speller_box.start(self.__text_edit.textCursor())


    @qtc.Slot()
    def __spell_check_finished_(self):
        """
        Called to inform this text dialog that spell checking has finished. This hides its spell
        checker box and clears any selection from its document's cursor.
        """
        self.__speller_box.hide()
        cursor = self.__text_edit.textCursor()
        cursor.clearSelection()
        self.__text_edit.setTextCursor(cursor)
