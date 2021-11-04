"""
Contains the BlockEditDock class.
"""
from ....Edit.CheckBoxEdit import CheckBoxEdit
from ....Edit.ComboEdit import ComboEdit
from ....Edit.HiddenEdit import HiddenEdit
from ....Edit.LineEdit import LineEdit
from ....Edit.TextEdit import TextEdit
from ....Settings import Settings
from ...Model.ProjectModel import ProjectModel
from .PlainTextEdit import PlainTextEdit
from PySide6.QtCore import (
    QModelIndex
    ,Qt
    ,Slot
)
from PySide6.QtWidgets import (
    QCheckBox
    ,QComboBox
    ,QDockWidget
    ,QFormLayout
    ,QHBoxLayout
    ,QLineEdit
    ,QMessageBox
    ,QPushButton
    ,QScrollArea
    ,QVBoxLayout
    ,QWidget
)
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
        Initializes this new block edit dock with the given optional Qt object
        parent.

        Parameters
        ----------
        parent : QObject
                 The optional Qt object parent.
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
        Sets this dock's project view to the one given. The given project view
        is not made the Qt child of this dock. If this dock currently has a view
        then it is unset from this dock.

        Parameters
        ----------
        view : ProjectView
               The project view.
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
                ,{edit._key_: edit._value_() for edit in self.__edits}
                ,Role.Properties
            )
            self.__applyButton.setEnabled(False)


    def __buildCheckbox_(
        self
        ,edit
        ,properties
    ):
        """
        Builds and returns a new Qt checkbox and label for adding to a Qt form
        layout from the given checkbox edit instance and block properties
        dictionary. The returned Qt checkbox has _value_ and _key_ attributes
        set, where _value_ is a function that returns its value and _key_ is a
        string for which block property the value belongs to.

        Parameters
        ----------
        edit : CheckBoxEdit
               The checkbox edit instance.
        properties : dictionary
                     The block properties.

        Returns
        -------
        widget : QCheckBox
                 The new Qt checkbox.
        label : string
                The label.
        """
        editWdgt = QCheckBox(edit.label())
        editWdgt.setCheckState(
            Qt.Checked if int(properties[edit.key()]) else Qt.Unchecked
        )
        editWdgt.stateChanged.connect(lambda : self.__applyButton.setEnabled(True))
        editWdgt._value_ = lambda e=editWdgt : str(int(e.checkState() == Qt.Checked))
        editWdgt._key_ = edit.key()
        return (editWdgt,"")


    def __buildCombo_(
        self
        ,edit
        ,properties
    ):
        """
        Builds and returns a new Qt combo box and label for the adding to a Qt
        form layout from the given combo box edit instance and block properties
        dictionary. The returned Qt combo box has _value_ and _key_ attributes
        set, where _value_ is a function that returns its value and _key_ is a
        string for which block property the value belongs to.

        Parameters
        ----------
        edit : ComboEdit
               The combo box edit instance.
        properties : dictionary
                     The block properties.

        Returns
        -------
        widget : QComboBox
                 The new Qt combo box.
        label : string
                The label.
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
        editWdgt._key_ = edit.key()
        return (editWdgt,edit.label())


    def __buildFormWidget_(
        self
        ,index
    ):
        """
        Builds and returns a Qt widget that contains all edit widgets for all
        properties of the block at the given Qt model index in this dock's
        project view, adding them to this dock's list of edit widgets. This
        clears any previous edit widgets from the last index that was built by
        this method.

        Parameters
        ----------
        index : QModelIndex
                The index.

        Returns
        -------
        result : QWidget
                 The Qt widget container.
        """
        self.__edits.clear()
        try:
            props = self.__view.model().data(index,Role.Properties)
            editDefs = self.__view.model().data(index,Role.EditDefs)
            layout = QFormLayout()
            layout.setLabelAlignment(Qt.AlignRight)
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
            ret.setContentsMargins(
                Settings.BlockEditDock.MARGIN_LEFT
                ,Settings.BlockEditDock.MARGIN_TOP
                ,Settings.BlockEditDock.MARGIN_RIGHT
                ,Settings.BlockEditDock.MARGIN_BOTTOM
            )
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
        Builds and returns a new Qt widget that is blank and not intended to be
        displayed on a Qt form layout from the given hidden edit instance. The
        returned Qt widget has _value_ and _key_ attributes set, where _value_
        is a function that returns its value and _key_ is a string for which
        block property the value belongs to.

        Parameters
        ----------
        edit : HiddenEdit
               The hidden edit instance.

        Returns
        -------
        result : QWidget
                 The new Qt widget.
        """
        editWdgt = QWidget()
        editWdgt._value_ = lambda val=edit.value() : val
        editWdgt._key_ = edit.key()
        return editWdgt


    def __buildLine_(
        self
        ,edit
        ,properties
    ):
        """
        Builds and returns a new Qt line edit widget and label for adding to a
        Qt form layout from the given line edit instance and block properties
        dictionary. The returned Qt line edit widget has _value_ and _key_
        attributes set, where _value_ is a function that returns its value and
        _key_ is a string for which block property the value belongs to.

        Parameters
        ----------
        edit : LineEdit
               The line edit instance.
        properties : dictionary
                     The block properties.

        Returns
        -------
        widget : QLineEdit
                 The new Qt line edit widget.
        label : string
                The label.
        """
        editWdgt = QLineEdit(properties[edit.key()])
        editWdgt.textChanged.connect(lambda : self.__applyButton.setEnabled(True))
        editWdgt._value_ = lambda e=editWdgt : e.text()
        editWdgt._key_ = edit.key()
        return (editWdgt,edit.label())


    def __buildText_(
        self
        ,edit
        ,properties
    ):
        """
        Builds and returns a new plain text edit widget and label for adding to
        a Qt form layout from the given text edit instance and block properties
        dictionary. The returned plain text edit widget has _value_ and _key_
        attributes set, where _value_ is a function that returns its value and
        _key_ is a string for which block property the value belongs to.

        Parameters
        ----------
        edit : TextEdit
               The text edit instance.
        properties : dictionary
                     The block properties.

        Returns
        -------
        widget : PlainTextEdit
                 The new plain text edit widget.
        label : string
                The label.
        """
        editWdgt = PlainTextEdit(spellCheck=edit.spellCheck(),popup=True)
        editWdgt.setPlainText(properties[edit.key()])
        editWdgt.textChanged.connect(lambda : self.__applyButton.setEnabled(True))
        editWdgt._value_ = lambda e=editWdgt : e.toPlainText()
        editWdgt._key_ = edit.key()
        return (editWdgt,edit.label())


    @Slot(QModelIndex)
    def __indexChanged_(
        self
        ,index
    ):
        """
        Called to update the block this dock is editing to the new one at the
        given Qt model index in this dock's project view. If the given index is
        invalid then this dock returns to a null state with no form.

        Garbage collection is required because rebuilding this dock's GUI causes
        large memory leaks otherwise.

        Parameters
        ----------
        index : QModelIndex
                The index.
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
        Called to update the index of the block this dock is editing to the
        given Qt model index in this dock's project view. It is assumed the same
        block is being edited so the edit widget form is not rebuilt.

        Parameters
        ----------
        index : QModelIndex
                The index.
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
