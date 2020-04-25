"""
Contains the TextDialog class.
"""
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import gui
from . import settings








class TextDialog(qtw.QDialog):
    """
    This is the text dialog class. It is a plain text editor with optional spell checking and
    correction. Any misspelled words are highlighted and a button is provided that begins spell
    checking and correction. All spell checking features can be enabled or disabled at
    initialization of the dialog.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(
        self
        ,text=""
        ,parent=None
        ,speller=False
        ):
        """
        Initializes a new text dialog with the given optional text and parent. Spelling features can
        also be enabled or disabled.

        Parameters
        ----------
        text : string
               The optional text of this new text dialog.
        parent : object
                 The optional qt object parent of this new text dialog.
        speller : bool
                  True to enable misspelled word highlighting and spell checking or false to disable
                  it all.
        """
        qtw.QDialog.__init__(self,parent)
        self.__speller = speller
        self.__textEdit = gui.PlainTextEdit(text,self,speller=speller,popup=False)
        self.__spellerBox = gui.SpellChecker("Spell Check",settings.DICTIONARY)
        self.__spellButton = qtw.QPushButton("Spell Check",self)
        self.__setupGui_()


    ####################
    # PUBLIC - Methods #
    ####################


    def text(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               The current of this text dialog.
        """
        return self.__textEdit.toPlainText()


    #######################
    # PROTECTED - Methods #
    #######################


    def closeEvent(
        self
        ,event
        ):
        """
        Implements the PySide2.QtWidgets.QWidget interface.

        Parameters
        ----------
        event : object
                See qt docs.
        """
        settings = qtc.QSettings()
        settings.setValue(self.__GEOMETRY_KEY,self.saveGeometry())
        event.accept()


    #######################
    # PRIVATE - Constants #
    #######################


    #
    # The key used to save this dialog's geometry using qt settings to make it persistent.
    #
    __GEOMETRY_KEY = "gui.dialog.text.geometry"


    #####################
    # PRIVATE - Methods #
    #####################


    def __restore_(
        self
        ):
        """
        Restores the geometry of this dialog.
        """
        settings = qtc.QSettings()
        geometry = settings.value(self.__GEOMETRY_KEY)
        if geometry:
            self.restoreGeometry(geometry)


    def __setupButtons_(
        self
        ):
        """
        Initializes the buttons of this new text dialog.

        Returns
        -------
        ret0 : PySide2.QtWidgets.QHBoxLayout
               A layout of initialized buttons.
        """
        set_ = qtw.QPushButton("Set")
        set_.clicked.connect(self.__set_)
        cancel = qtw.QPushButton("Cancel")
        cancel.clicked.connect(self.__cancel_)
        self.__spellButton.clicked.connect(self.__spellCheck_)
        self.__spellButton.setDisabled(not self.__speller)
        ret = qtw.QHBoxLayout()
        ret.addWidget(set_)
        ret.addWidget(cancel)
        ret.addStretch()
        ret.addWidget(self.__spellButton)
        return ret


    def __setupGui_(
        self
        ):
        """
        Initializes the GUI of this new text dialog.
        """
        layout = qtw.QVBoxLayout()
        layout.addWidget(self.__textEdit)
        layout.addWidget(self.__setupSpellChecker_())
        layout.addLayout(self.__setupButtons_())
        self.setLayout(layout)
        self.__restore_()


    def __setupSpellChecker_(
        self
        ):
        """
        Initializes the spell checker box of this new text dialog.

        Returns
        -------
        ret0 : socref.gui.SpellChecker
               The initialized spell checker box widget.
        """
        self.__spellerBox.hide()
        self.__spellerBox.cursorChanged.connect(
            lambda cursor : self.__textEdit.setTextCursor(cursor)
        )
        self.__spellerBox.finished.connect(self.__spellCheckFinished_)
        return self.__spellerBox


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot()
    def __cancel_(
        self
        ):
        """
        Called to set this dialog's done status to rejected and then closing it.
        """
        self.done(qtw.QDialog.Rejected)
        self.close()


    @qtc.Slot()
    def __set_(
        self
        ):
        """
        Called to set this dialog's done status to accepted and then closing it.
        """
        self.done(qtw.QDialog.Accepted)
        self.close()


    @qtc.Slot()
    def __spellCheck_(
        self
        ):
        """
        Called to begin spell checking this text dialog's document. This shows its spell checker box
        and tells it to start checking.
        """
        self.__spellerBox.show()
        self.__spellerBox.start(self.__textEdit.textCursor())


    @qtc.Slot()
    def __spellCheckFinished_(
        self
        ):
        """
        Called to inform this text dialog that spell checking has finished. This hides its spell
        checker box and clears any selection from its document's cursor.
        """
        self.__spellerBox.hide()
        cursor = self.__textEdit.textCursor()
        cursor.clearSelection()
        self.__textEdit.setTextCursor(cursor)
