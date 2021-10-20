"""
Contains the PlainTextEdit class.
"""
from ..Controller.SpellHighlighter import SpellHighlighter
from ..Dialog import TextDialog
from PySide6.QtCore import Qt
from PySide6.QtCore import Slot
from PySide6.QtGui import QAction
from PySide6.QtWidgets import QPlainTextEdit




class PlainTextEdit(QPlainTextEdit):
    """
    This is the plain text class. It provides additional functionality to its
    inherited class. Spell checking highlights misspelled words. A popup
    shortcut is provided to open a larger text editor dialog that has additional
    spell checking functionality. Spell checking and the editor dialog popup
    shortcut features can be enabled or disabled.
    """


    def __init__(
        self
        ,text=""
        ,parent=None
        ,speller=False
        ,popup=False
    ):
        """
        Initializes this new plain text editor with the given optional text, Qt
        object parent, spell checking flag, and popup shortcut flag. By default
        spell checking and the popup shortcut are disabled.

        Parameters
        ----------
        text : string
               The optional text.
        parent : QObject
                 The optional Qt object parent.
        speller : bool
                  True to enable spell checking or false to disable it.
        popup : bool
                True to enable the popup shortcut or false to disable it.
        """
        super().__init__(text,parent)
        self.__speller = speller
        self.__popup = popup
        self.__highlighter = None
        if speller:
            self.setSpellerEnabled(True)
        self.__setupActions_()


    def setSpellerEnabled(
        self
        ,enabled
    ):
        """
        Sets the state of this editor's spelling checking to given flag.

        Parameters
        ----------
        enabled : bool
                  True to enable spell checking or false to disable it.
        """
        if not enabled:
            if self.__highlighter is not None:
                self.__highlighter.deleteLater()
                self.__highlighter = None
        elif self.__highlighter is None:
            self.__highlighter = SpellHighlighter(self.document())
        self.__speller = enabled


    @Slot()
    def __dialog_(
        self
    ):
        """
        Called to open a modal dialog text editor to edit this editor's text.
        """
        if self.__popup:
            dialog = TextDialog.TextDialog(self.toPlainText(),self,speller=self.__speller)
            dialog.setWindowTitle("Text Editor - Socrates' Reference")
            if dialog.exec():
                self.setPlainText(dialog.text())


    def __setupActions_(
        self
    ):
        """
        Initialize the Qt action shortcuts of this new text editor.
        """
        dialog = QAction(self)
        dialog.setShortcutContext(Qt.WidgetShortcut)
        dialog.setShortcut(Qt.CTRL + Qt.Key_E)
        dialog.triggered.connect(self.__dialog_)
        self.addAction(dialog)
