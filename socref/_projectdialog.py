"""
Contains the ProjectDialog class.
"""
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import settings








class ProjectDialog(qtw.QDialog):
    """
    This is the project dialog class. It is a basic form dialog that allows the user to edit the
    project name and parse path of the model it is given on initialization. It is a persistent
    dialog that remembers its geometry.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(
        self
        ,model
        ,parent=None
        ):
        """
        Initializes a new project dialog with the given model and optional parent.

        Parameters
        ----------
        model : socref.model.ProjectModel
                The project model that this dialog edits with its form.
        parent : object
                 The optional qt object parent of this dialog.
        """
        qtw.QDialog.__init__(self,parent)
        self.__nameEdit = qtw.QLineEdit(self)
        self.__parsePathEdit = qtw.QLineEdit(self)
        self.__model = model
        model.nameChanged.connect(self.__nameChanged_)
        model.parsePathChanged.connect(self.__parsePathChanged_)
        self.__setupGui_()


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
        Initializes the buttons of this new dialog.
        """
        ok = qtw.QPushButton("Ok")
        ok.clicked.connect(self.__ok_)
        apply_ = qtw.QPushButton("Apply")
        apply_.clicked.connect(self.__apply_)
        cancel = qtw.QPushButton("Cancel")
        cancel.clicked.connect(self.__cancel_)
        ret = qtw.QHBoxLayout()
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
        ret = qtw.QFormLayout()
        ret.addRow("Project Name:",self.__nameEdit)
        ret.addRow("Parsing Path:",self.__parsePathEdit)
        return ret


    def __setupGui_(
        self
        ):
        """
        Initializes the GUI of this new dialog.
        """
        layout = qtw.QVBoxLayout()
        layout.addLayout(self.__setupForm_())
        layout.addLayout(self.__setupButtons_())
        self.setLayout(layout)
        self.setWindowTitle("Project Properties")
        self.__restore_()


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot()
    def __apply_(
        self
        ):
        """
        Called to apply this dialog's edit widget values to its project model.
        """
        self.__model.setName(self.__nameEdit.text())
        self.__model.setParsePath(self.__parsePathEdit.text())


    @qtc.Slot()
    def __cancel_(
        self
        ):
        """
        Called to cancel this dialog, closing it and not applying any changes made in its edit
        widgets.
        """
        self.done(qtw.QDialog.Rejected)
        self.close()


    @qtc.Slot(str)
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


    @qtc.Slot()
    def __ok_(
        self
        ):
        """
        Called to apply this dialog's edit widget values to its project model and then close itself.
        """
        self.__apply_()
        self.done(qtw.QDialog.Accepted)
        self.close()


    @qtc.Slot(str)
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


    #######################
    # PRIVATE - Constants #
    #######################


    #
    # The key used to save this dialog's geometry using qt settings to make it persistent.
    #
    __GEOMETRY_KEY = "gui.dialog.project.geometry"
