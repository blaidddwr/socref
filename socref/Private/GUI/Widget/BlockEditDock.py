"""
Contains the BlockEditDock class.
"""
from ....Edit.CheckBoxEdit import *
from ....Edit.ComboEdit import *
from ....Edit.HiddenEdit import *
from ....Edit.LineEdit import *
from ....Edit.TextEdit import *
from ...Model.ProjectModel import *
from .PlainTextEdit import *
from PySide6.QtCore import QModelIndex
from PySide6.QtCore import Qt
from PySide6.QtCore import Slot
from PySide6.QtWidgets import QCheckBox
from PySide6.QtWidgets import QComboBox
from PySide6.QtWidgets import QDockWidget
from PySide6.QtWidgets import QFormLayout
from PySide6.QtWidgets import QHBoxLayout
from PySide6.QtWidgets import QLineEdit
from PySide6.QtWidgets import QMessageBox
from PySide6.QtWidgets import QPushButton
from PySide6.QtWidgets import QScrollArea
from PySide6.QtWidgets import QVBoxLayout
from PySide6.QtWidgets import QWidget
from gc import collect as gCollect




class BlockEditDock(QDockWidget):
    """
    This is the block edit dock class. It interfaces itself to a project view.
    It connects the appropriate signal to listen for the current index of the
    view to change. When the index does change, it updates itself to reflect the
    new block that can be edited by the user. It provides a GUI edit form to the
    user for modifying a block's properties. The specific edit elements in the
    form are built using the block's edit definitions, adding them all in the
    form in the same order as the edit definitions list. An apply button is
    added to the end of the form to update the block's properties to the current
    values of all the edit widgets. If the current index is changed and there
    are unsaved changes to the properties this will ask the user if they want to
    save the changes.
    """


    def __init__(
        self
        ,parent=None
    ):
        """
        Initialize a new block edit dock with the given optional parent.

        Parameters
        ----------
        parent : object
                 Optional qt object parent of this new block edit dock.
        """
        super().__init__(parent)
        self.__area = QScrollArea(widgetResizable=True)
        self.__applyButton = QPushButton("Apply")
        self.__index = QModelIndex()
        self.__view = None
        self.__edits = []
        self.__setupGui_()


    def setView(
        self
        ,view
    ):
        """
        Sets this dock's project view to the one given. If this dock currently
        has a view it is disconnected from this dock.

        Parameters
        ----------
        view : socref.Private.GUI.Widget.ProjectView
               The newly interfaced project view of this dock. This project view
               is not made the child of this dock.
        """
        if self.__view is not None:
            self.__view.current_changed.disconnect(self.__indexChanged_)
            self.__view.current_changed.disconnect(self.__indexMoved_)
            self.__view.current_changed.disconnect(self.__indexRemoved_)
        self.__view = view
        self.__view.indexChanged.connect(self.__indexChanged_)
        self.__view.indexMoved.connect(self.__indexMoved_)
        self.__view.indexRemoved.connect(self.__indexRemoved_)


    @Slot()
    def __apply_(
        self
    ):
        """
        Called to set this dock's currently indexed block's properties to the
        current values of its edit widgets. If the current index is not valid
        then this does nothing.
        """
        if self.__index.isValid():
            self.__view.model().setData(
                self.__index
                ,{edit._key: edit._value_() for edit in self.__edits}
                ,Role.Properties
            )
            self.__applyButton.setEnabled(False)


    def __buildCheckbox_(
        self
        ,edit
        ,properties
    ):
        """
        Getter method.

        Parameters
        ----------
        edit : socref.Edit.CheckBoxEdit
               The edit class instance used to build the returned edit widget.
        properties : dictionary
                     The properties of the block used to get the initial value
                     of the returned edit widget.

        Returns
        -------
        widget : PySide2.QtWidgets.QCheckBox
                 A new checkbox configured for the given definition and
                 properties.
        label : string
                A label for adding it to a form.
        """
        editWdgt = QCheckBox(edit.label())
        editWdgt.setCheckState(
            Qt.Checked if int(properties[edit.key()]) else Qt.Unchecked
        )
        editWdgt.stateChanged.connect(lambda : self.__applyButton.setEnabled(True))
        editWdgt._value_ = lambda e=editWdgt : str(int(e.checkState() == Qt.Checked))
        editWdgt._key = edit.key()
        return (editWdgt,"")


    def __buildCombo_(
        self
        ,edit
        ,properties
    ):
        """
        Getter method.

        Parameters
        ----------
        edit : socref.Edit.ComboEdit
               The edit class instance used to build the returned edit widget.
        properties : dictionary
                     The properties of the block used to get the initial value
                     of the returned edit widget.

        Returns
        -------
        widget : PySide2.QtWidgets.QComboBox
                 A new combo box configured for the given definition and
                 properties.
        label : string
                A label for adding it to a form.
        """
        editWdgt = QComboBox()
        for (icon,text) in edit.selections():
            if icon:
                editWdgt.addItem(icon,text)
            else:
                editWdgt.addItem(text)
        editWdgt.setCurrentText(properties[edit.key()])
        editWdgt.currentTextChanged.connect(lambda : self.__applyButton.setEnabled(True))
        editWdgt._value_ = lambda e=editWdgt : e.currentText()
        editWdgt._key = edit.key()
        return (editWdgt,edit.label())


    def __buildFormWidget_(
        self
        ,index
    ):
        """
        Builds and initializes all edit widgets for all properties of the block
        at the given index based off its edit definitions, adding them to this
        dock's list of edit widgets. This clears any previous edit widgets from
        the last index that was built by this method.

        Parameters
        ----------
        index : PySide2.QtCore.QModelIndex
                The index of the block whose property edit widgets are built.

        Returns
        -------
        result : PySide2.QtWidgets.QWidget
                 Contains all built edit widgets.
        """
        self.__edits.clear()
        try:
            props = self.__view.model().data(index,Role.Properties)
            editDefs = self.__view.model().data(index,Role.EditDefs)
            layout = QFormLayout()
            for edit in editDefs:
                editWdgt = None
                label = None
                if isinstance(edit,LineEdit):
                    (editWdgt,label) = self.__buildLine_(edit,props)
                elif isinstance(edit,TextEdit):
                    (editWdgt,label) = self.__buildText_(edit,props)
                elif isinstance(edit,CheckBoxEdit):
                    (editWdgt,label) = self.__buildCheckbox_(edit,props)
                elif isinstance(edit,ComboEdit):
                    (editWdgt,label) = self.__buildCombo_(edit,props)
                elif isinstance(edit,HiddenEdit):
                    editWdgt = self.__buildHidden_(edit)
                else:
                    raise RuntimeError("Unknown edit class.")
                if label is not None:
                    layout.addRow(label,editWdgt)
                self.__edits.append(editWdgt)
            ret = QWidget()
            ret.setContentsMargins(0,16,0,4)
            ret.setLayout(layout)
            return ret
        except:
            self.__edits.clear()
            raise


    def __buildHidden_(
        self
        ,edit
    ):
        """
        Getter method.

        Parameters
        ----------
        edit : socref.Edit.HiddenEdit
               The edit class instance used to build the returned edit widget.

        Returns
        -------
        result : PySide2.QtWidgets.QWidget
                 A new hidden edit widget configured for the given definition.
        """
        editWdgt = QWidget()
        editWdgt._value_ = lambda val=edit.value() : val
        editWdgt._key = edit.key()
        return editWdgt


    def __buildLine_(
        self
        ,edit
        ,properties
    ):
        """
        Getter method.

        Parameters
        ----------
        edit : socref.Edit.LineEdit
               The edit class instance used to build the returned edit widget.
        properties : dictionary
                     The properties of the block used to get the initial value
                     of the returned edit widget.

        Returns
        -------
        widget : PySide2.QtWidgets.QLineEdit
                 A new line editor configured for the given definition and
                 properties.
        label : string
                A label for adding it to a form.
        """
        editWdgt = QLineEdit(properties[edit.key()])
        editWdgt.textChanged.connect(lambda : self.__applyButton.setEnabled(True))
        editWdgt._value_ = lambda e=editWdgt : e.text()
        editWdgt._key = edit.key()
        return (editWdgt,edit.label())


    def __buildText_(
        self
        ,edit
        ,properties
    ):
        """
        Getter method.

        Parameters
        ----------
        edit : socref.Edit.TextEdit
               The edit class instance used to build the returned edit widget.
        properties : dictionary
                     The properties of the block used to get the initial value
                     of the returned edit widget.

        Returns
        -------
        widget : socref.Private.GUI.Widget.PlainTextEdit
                 A new text editor configured for the given definition and
                 properties.
        label : string
                A label for adding it to a form.
        """
        editWdgt = PlainTextEdit(speller=edit.spellCheck(),popup=True)
        editWdgt.setPlainText(properties[edit.key()])
        editWdgt.textChanged.connect(lambda : self.__applyButton.setEnabled(True))
        editWdgt._value_ = lambda e=editWdgt : e.toPlainText()
        editWdgt._key = edit.key()
        return (editWdgt,edit.label())


    @Slot(QModelIndex)
    def __indexChanged_(
        self
        ,index
    ):
        """
        Called to update the block this dock is editing to the new one at the
        given index. If the given index is invalid then this dock returns to a
        null state with no form.

        Garbage collection is required because rebuilding this dock's GUI causes
        large memory leaks otherwise.

        Parameters
        ----------
        index : PySide2.QtCore.QModelIndex
                The index of the new block whose properties are edited by this
                dock.
        """
        if self.__index.isValid() and self.__applyButton.isEnabled():
            answer = QMessageBox.question(
                self
                ,"Unsaved Changes"
                ,"The current block has unsaved modifications. Discarding will cause modifications"
                 " to be lost!"
                ,QMessageBox.Save|QMessageBox.Discard
            )
            if answer == QMessageBox.Save:
                self.__apply_()
        self.__index = index
        if self.__area.widget():
            self.__area.widget().deleteLater()
            gCollect()
        if index.isValid():
            self.__area.setWidget(self.__buildFormWidget_(index))
            self.__applyButton.setEnabled(False)
        else:
            self.__edits.clear()
            self.__area.setWidget(None)


    @Slot(QModelIndex)
    def __indexMoved_(
        self
        ,index
    ):
        """
        Called to update the index of the block this dock is editing. It is
        assumed the same block is being edited so the edit widget form is not
        rebuilt.

        Parameters
        ----------
        index : PySide2.QtCore.QModelIndex
                The new index of the block this dock is editing.
        """
        self.__index = index


    @Slot()
    def __indexRemoved_(
        self
    ):
        """
        Called to remove this dock's current index, setting it to invalid.
        """
        self.__index = QModelIndex()


    def __setupGui_(
        self
    ):
        """
        Initializes the GUI of this new block edit dock.
        """
        self.__applyButton.clicked.connect(self.__apply_)
        self.__applyButton.setEnabled(False)
        bottom = QHBoxLayout()
        bottom.addWidget(self.__applyButton)
        bottom.addStretch()
        layout = QVBoxLayout()
        layout.addWidget(self.__area)
        layout.addLayout(bottom)
        central = QWidget()
        central.setLayout(layout)
        self.setWidget(central)


Role = ProjectModel.Role
