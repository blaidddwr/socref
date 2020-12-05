"""
Contains the PlainTextEdit class.
"""
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import gui
from . import settings








class PlainTextEdit(qtw.QPlainTextEdit):
    """
    This is the plain text class. It provides additional functionality to its
    inherited class. Misspelled words are highlighted. A shortcut is provided to
    open a larger text editor dialog that has additional spell checking
    functionality. Misspelled word highlighting and the editor dialog popup
    features can be enabled or disabled.
    """


    def __init__(
        self
        ,text=""
        ,parent=None
        ,speller=False
        ,popup=False
        ):
        """
        Initializes a new plain text editor with the given optional text and
        parent. Spelling and dialog popup can also be enabled or disabled.

        Parameters
        ----------
        text : string
               The optional edited text of this new plain text editor.
        parent : object
                 Optional qt object parent of this new plain text editor.
        speller : bool
                  True to enable misspelled word highlighting or false to
                  disable it.
        popup : bool
                True to enable the popup edit dialog shortcut or false to
                disable it.
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
        Sets the state of this editor's spelling highlighter to enabled or
        disabled.

        Parameters
        ----------
        enabled : bool
                  True to enable this editor's spelling highlighter or false to
                  disable it.
        """
        if not enabled:
            if self.__highlighter is not None:
                self.__highlighter.deleteLater()
                self.__highlighter = None
        elif self.__highlighter is None:
            self.__highlighter = gui.SpellHighlighter(settings.DICTIONARY,self.document())
        self.__speller = enabled


    @qtc.Slot()
    def __dialog_(
        self
        ):
        """
        Called to open a modal dialog text editor to edit this editor's text.
        """
        if self.__popup:
            dialog = gui.TextDialog(self.toPlainText(),self,speller=self.__speller)
            dialog.setWindowTitle("Text Editor - Socrates' Reference")
            if dialog.exec_():
                self.setPlainText(dialog.text())


    def __setupActions_(
        self
        ):
        """
        Initialize the qt action shortcuts of this new text editor.
        """
        dialog = qtw.QAction(self)
        dialog.setShortcutContext(qtc.Qt.WidgetShortcut)
        dialog.setShortcut(qtc.Qt.CTRL + qtc.Qt.Key_E)
        dialog.triggered.connect(self.__dialog_)
        self.addAction(dialog)
