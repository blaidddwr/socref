"""
Contains the CodeDialog class.
"""
from PySide2 import QtCore as qtc
from PySide2 import QtGui as qtg
from PySide2 import QtWidgets as qtw
from . import settings








class CodeDialog(qtw.QDialog):
    """
    This is the code dialog class. It displays any unknown code fragments given
    to it. The fragments must be organized into a flat dictionary where the key
    should provide a meaningful identity about the code fragment to the user.
    The user is provided a list and read only text view to navigate the
    fragments, along with a copy to clipboard button.
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
        optional parent.

        Parameters
        ----------
        code : dictionary
               The code that this dialog displays to the user.
        parent : object
                 The optional qt object parent of this dialog.
        """
        qtw.QDialog.__init__(self,parent)
        self.__code = code
        self.__splitter = qtw.QSplitter(self)
        self.__view = qtw.QPlainTextEdit(self,readOnly=True)
        self.__setupGui_()


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
        settings.setValue(self.__STATE_KEY,self.__splitter.saveState())
        event.accept()


    def __restore_(
        self
        ):
        """
        Restores the geometry of this dialog and the state of this dialog's qt
        splitter.
        """
        settings = qtc.QSettings()
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
        close = qtw.QPushButton("Close")
        close.clicked.connect(self.__close_)
        copy = qtw.QPushButton("Copy to Clipboard")
        copy.clicked.connect(self.__copy_)
        ret = qtw.QHBoxLayout()
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
        layout = qtw.QVBoxLayout()
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
        list_ = qtw.QListWidget()
        list_.currentTextChanged.connect(self.__itemChanged_)
        list_.addItems(list(self.__code.keys()))
        self.__splitter.addWidget(list_)
        self.__splitter.addWidget(self.__view)
        return self.__splitter


    @qtc.Slot()
    def __close_(
        self
        ):
        """
        Called to close this dialog.
        """
        self.done(qtw.QDialog.Accepted)
        self.close()


    @qtc.Slot()
    def __copy_(
        self
        ):
        """
        Called to copy this dialog's current code fragment to the system
        clipboard.
        """
        clipboard = qtg.QClipboard()
        clipboard.setText(self.__view.toPlainText())


    @qtc.Slot(str)
    def __itemChanged_(
        self
        ,key
        ):
        """
        Called to inform this dialog's the selected code fragment has changed to
        the one with the given key.

        Parameters
        ----------
        key : string
              The new key of the selected code fragment.
        """
        self.__view.setPlainText(self.__code[key])
