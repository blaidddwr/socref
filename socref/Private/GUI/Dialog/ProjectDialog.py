"""
Contains the ProjectDialog class.
"""
from PySide6.QtCore import QSettings
from PySide6.QtCore import Slot
from PySide6.QtWidgets import QDialog
from PySide6.QtWidgets import QFormLayout
from PySide6.QtWidgets import QHBoxLayout
from PySide6.QtWidgets import QLineEdit
from PySide6.QtWidgets import QPushButton
from PySide6.QtWidgets import QVBoxLayout




class ProjectDialog(QDialog):
    """
    This is the project dialog class. It is a basic form dialog that allows the
    user to edit the project name and parse path of the model it is given on
    initialization. It is a persistent dialog that remembers its geometry.
    """
    __GEOMETRY_KEY = "gui.dialog.project.geometry"


    def __init__(
        self
        ,model
        ,parent=None
    ):
        """
        Initializes a new project dialog with the given model and optional
        parent.

        Parameters
        ----------
        model : ProjectModel
                The project model that this dialog edits with its form.
        parent : object
                 The optional Qt object parent of this dialog.
        """
        super().__init__(parent)
        self.__nameEdit = QLineEdit(self)
        self.__parsePathEdit = QLineEdit(self)
        self.__model = model
        model.nameChanged.connect(self.__nameChanged_)
        model.parsePathChanged.connect(self.__parsePathChanged_)
        self.__setupGui_()


    def closeEvent(
        self
        ,event
    ):
        settings = QSettings()
        settings.setValue(self.__GEOMETRY_KEY,self.saveGeometry())
        event.accept()


    @Slot()
    def __apply_(
        self
    ):
        """
        Called to apply this dialog's edit widget values to its project model.
        """
        self.__model.setName(self.__nameEdit.text())
        self.__model.setParsePath(self.__parsePathEdit.text())


    @Slot()
    def __cancel_(
        self
    ):
        """
        Called to cancel this dialog, closing it and not applying any changes
        made in its edit widgets.
        """
        self.done(QDialog.Rejected)
        self.close()


    @Slot(str)
    def __nameChanged_(
        self
        ,name
    ):
        """
        Called to inform this dialog its project model's name has changed.

        Parameters
        ----------
        name : string
               The new name of this dialog's project model.
        """
        self.__nameEdit.setText(name)


    @Slot()
    def __ok_(
        self
    ):
        """
        Called to apply this dialog's edit widget values to its project model
        and then close itself.
        """
        self.__apply_()
        self.done(QDialog.Accepted)
        self.close()


    @Slot(str)
    def __parsePathChanged_(
        self
        ,path
    ):
        """
        Called to inform this dialog its project model's parse path has changed.

        Parameters
        ----------
        path : string
               The new parse path of this dialog's project model.
        """
        self.__parsePathEdit.setText(path)


    def __restore_(
        self
    ):
        """
        Restores the geometry of this dialog.
        """
        settings = QSettings()
        geometry = settings.value(self.__GEOMETRY_KEY)
        if geometry:
            self.restoreGeometry(geometry)


    def __setupButtons_(
        self
    ):
        """
        Initializes the buttons of this new dialog.
        """
        ok = QPushButton("Ok")
        ok.clicked.connect(self.__ok_)
        apply_ = QPushButton("Apply")
        apply_.clicked.connect(self.__apply_)
        cancel = QPushButton("Cancel")
        cancel.clicked.connect(self.__cancel_)
        ret = QHBoxLayout()
        ret.addWidget(ok)
        ret.addWidget(apply_)
        ret.addStretch()
        ret.addWidget(cancel)
        return ret


    def __setupForm_(
        self
    ):
        """
        Initializes the form of this new dialog.
        """
        self.__nameEdit.setText(self.__model.name())
        self.__parsePathEdit.setText(self.__model.parsePath())
        ret = QFormLayout()
        ret.addRow("Project Name:",self.__nameEdit)
        ret.addRow("Parsing Path:",self.__parsePathEdit)
        return ret


    def __setupGui_(
        self
    ):
        """
        Initializes the GUI of this new dialog.
        """
        layout = QVBoxLayout()
        layout.addLayout(self.__setupForm_())
        layout.addLayout(self.__setupButtons_())
        self.setLayout(layout)
        self.setWindowTitle("Project Properties")
        self.__restore_()
