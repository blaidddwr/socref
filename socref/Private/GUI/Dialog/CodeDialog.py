"""
Contains the CodeDialog class.
"""
from PySide6.QtCore import QSettings
from PySide6.QtCore import Slot
from PySide6.QtGui import QClipboard
from PySide6.QtWidgets import QDialog
from PySide6.QtWidgets import QHBoxLayout
from PySide6.QtWidgets import QListWidget
from PySide6.QtWidgets import QPlainTextEdit
from PySide6.QtWidgets import QPushButton
from PySide6.QtWidgets import QSplitter
from PySide6.QtWidgets import QVBoxLayout




class CodeDialog(QDialog):
    """
    This is the code dialog class. It displays any unknown code fragments given
    to it. It is a persistent dialog that remembers its geometry. The user is
    provided a list and read only text view to navigate the fragments, along
    with a copy to clipboard button.
    """
    __GEOMETRY_KEY = "gui.dialog.code.geometry"
    __STATE_KEY = "gui.dialog.code.state"


    def __init__(
        self
        ,code
        ,parent=None
    ):
        """
        Initializes a new code dialog with the given code dictionary and
        optional Qt object parent. The given code dictionary must be organized
        as a dictionary where the keys are meaningful string identifiers and the
        values are lists of strings where each string is a line of code.

        Parameters
        ----------
        code : dictionary
               The code dictionary.
        parent : object
                 The optional Qt object parent.
        """
        super().__init__(parent)
        self.__code = code
        self.__splitter = QSplitter(self)
        self.__view = QPlainTextEdit(self,readOnly=True)
        self.__setupGui_()


    def closeEvent(
        self
        ,event
    ):
        settings = QSettings()
        settings.setValue(self.__GEOMETRY_KEY,self.saveGeometry())
        settings.setValue(self.__STATE_KEY,self.__splitter.saveState())
        event.accept()


    @Slot()
    def __close_(
        self
    ):
        """
        Called to close this dialog.
        """
        self.done(QDialog.Accepted)
        self.close()


    @Slot()
    def __copy_(
        self
    ):
        """
        Called to copy this dialog's current code fragment to the system
        clipboard.
        """
        clipboard = QClipboard()
        clipboard.setText(self.__view.toPlainText())


    @Slot(str)
    def __itemChanged_(
        self
        ,key
    ):
        """
        Called to inform this dialog's selected code fragment has changed to the
        one with the given key.

        Parameters
        ----------
        key : string
              The key.
        """
        self.__view.setPlainText("\n".join(self.__code[key]))


    def __restore_(
        self
    ):
        """
        Restores the geometry of this dialog and the state of this dialog's Qt
        splitter.
        """
        settings = QSettings()
        geometry = settings.value(self.__GEOMETRY_KEY)
        state = settings.value(self.__STATE_KEY)
        if geometry:
            self.restoreGeometry(geometry)
        if state:
            self.__splitter.restoreState(state)


    def __setupButtons_(
        self
    ):
        """
        Initializes the GUI of this new dialog.
        """
        close = QPushButton("Close")
        close.clicked.connect(self.__close_)
        copy = QPushButton("Copy to Clipboard")
        copy.clicked.connect(self.__copy_)
        ret = QHBoxLayout()
        ret.addWidget(close)
        ret.addStretch()
        ret.addWidget(copy)
        return ret


    def __setupGui_(
        self
    ):
        """
        Initializes the GUI of this new dialog.
        """
        layout = QVBoxLayout()
        layout.addWidget(self.__setupView_())
        layout.addLayout(self.__setupButtons_())
        self.setLayout(layout)
        self.setWindowTitle("Unknown Code Fragments")
        self.__restore_()


    def __setupView_(
        self
    ):
        """
        Initializes the GUI of this new dialog.
        """
        list_ = QListWidget()
        list_.currentTextChanged.connect(self.__itemChanged_)
        list_.addItems(list(self.__code.keys()))
        self.__splitter.addWidget(list_)
        self.__splitter.addWidget(self.__view)
        return self.__splitter
