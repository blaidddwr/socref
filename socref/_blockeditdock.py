"""
Contains the BlockEditDock class.
"""
import gc
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import core
from . import gui








class BlockEditDock(qtw.QDockWidget):
    """
    This is the block edit dock class. It attaches itself to a project view. It
    provides a GUI edit form to the user for modifying a block's properties.

    It attaches itself to a project view. It connects the appropriate signal to
    listen for the current index of the view to change. When the index does
    change, it updates itself to reflect the new block that can be edited by the
    user.

    It provides a GUI edit form to the user for modifying a block's properties.
    The specific edit elements in the form is built based off the block's edit
    definitions, adding them all in the form in the same order as the edit
    definitions list. An apply button is also added to the end of the form to
    update the block's properties to the current values of all the edit widgets.
    If the current index is changed and there are unsaved changes to the
    properties this will ask the user if they want to save the changes.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


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
        qtw.QDockWidget.__init__(self,parent)
        self.__area = qtw.QScrollArea(widgetResizable=True)
        self.__applyButton = qtw.QPushButton("Apply")
        self.__index = qtc.QModelIndex()
        self.__view = None
        self.__edits = []
        self.__setupGui_()


    ####################
    # PUBLIC - Methods #
    ####################


    def setView(
        self
        ,view
        ):
        """
        Sets this dock's view to the one given. If this dock currently has a
        view it is disconnected from this dock.

        Parameters
        ----------
        view : socref.gui.ProjectView
               The new attached view of this dock.
        """
        if self.__view is not None:
            self.__view.current_changed.disconnect(self.__indexChanged_)
            self.__view.current_changed.disconnect(self.__indexMoved_)
            self.__view.current_changed.disconnect(self.__indexRemoved_)
        self.__view = view
        self.__view.indexChanged.connect(self.__indexChanged_)
        self.__view.indexMoved.connect(self.__indexMoved_)
        self.__view.indexRemoved.connect(self.__indexRemoved_)


    #####################
    # PRIVATE - Methods #
    #####################


    def __buildCheckbox_(
        self
        ,definition
        ,properties
        ):
        """
        Getter method.

        Parameters
        ----------
        definition : dictionary
                     The edit definition used to build the returned edit widget.
        properties : dictionary
                     The properties of the block used to get the initial value
                     of the returned edit widget.

        Returns
        -------
        ret0 : PySide2.QtWidgets.QCheckBox
               A new checkbox configured for the given definition and
               properties.
        ret1 : string
               A label for adding it to a form.
        """
        edit = qtw.QCheckBox(definition["label"])
        edit.setCheckState(
            qtc.Qt.Checked if int(properties[definition["key"]]) else qtc.Qt.Unchecked
        )
        edit.stateChanged.connect(lambda : self.__applyButton.setEnabled(True))
        edit._value_ = lambda e=edit : str(int(e.checkState() == qtc.Qt.Checked))
        edit._key = definition["key"]
        return (edit,"")


    def __buildCombo_(
        self
        ,definition
        ,properties
        ):
        """
        Getter method.

        Parameters
        ----------
        definition : dictionary
                     The edit definition used to build the returned edit widget.
        properties : dictionary
                     The properties of the block used to get the initial value
                     of the returned edit widget.

        Returns
        -------
        ret0 : PySide2.QtWidgets.QComboBox
               A new combo box configured for the given definition and
               properties.
        ret1 : string
               A label for adding it to a form.
        """
        edit = qtw.QComboBox()
        for selection in definition["selections"]:
            if "icon" in selection:
                edit.addItem(selection["icon"],selection["text"])
            else:
                edit.addItem(selection["text"])
        edit.setCurrentText(properties[definition["key"]])
        edit.currentTextChanged.connect(lambda : self.__applyButton.setEnabled(True))
        edit._value_ = lambda e=edit : e.currentText()
        edit._key = definition["key"]
        return (edit,definition["label"])


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
        ret0 : PySide2.QtWidgets.QWidget
               Contains all built edit widgets.
        """
        self.__edits.clear()
        try:
            props = self.__view.model().data(index,core.Role.Properties)
            defs = self.__view.model().data(index,core.Role.EditDefs)
            layout = qtw.QFormLayout()
            for def_ in defs:
                edit = None
                label = None
                if def_["type"] == "line":
                    (edit,label) = self.__buildLine_(def_,props)
                elif def_["type"] == "text":
                    (edit,label) = self.__buildText_(def_,props)
                elif def_["type"] == "checkbox":
                    (edit,label) = self.__buildCheckbox_(def_,props)
                elif def_["type"] == "combobox":
                    (edit,label) = self.__buildCombo_(def_,props)
                elif def_["type"] == "hidden":
                    edit = self.__buildHidden_(def_)
                else:
                    raise RuntimeError("Unknown edit definition.")
                if label is not None:
                    layout.addRow(label,edit)
                self.__edits.append(edit)
            ret = qtw.QWidget()
            ret.setContentsMargins(0,16,0,4)
            ret.setLayout(layout)
            return ret
        except:
            self.__edits.clear()
            raise


    def __buildHidden_(
        self
        ,definition
        ):
        """
        Getter method.

        Parameters
        ----------
        definition : dictionary
                     The edit definition used to build the returned edit widget.

        Returns
        -------
        ret0 : PySide2.QtWidgets.QWidget
               A new hidden edit widget configured for the given definition.
        """
        edit = qtw.QWidget()
        edit._value_ = lambda val=definition["value"] : val
        edit._key = definition["key"]
        return edit


    def __buildLine_(
        self
        ,definition
        ,properties
        ):
        """
        Getter method.

        Parameters
        ----------
        definition : dictionary
                     The edit definition used to build the returned edit widget.
        properties : dictionary
                     The properties of the block used to get the initial value
                     of the returned edit widget.

        Returns
        -------
        ret0 : PySide2.QtWidgets.QLineEdit
               A new line editor configured for the given definition and
               properties.
        ret1 : string
               A label for adding it to a form.
        """
        edit = qtw.QLineEdit(properties[definition["key"]])
        edit.textChanged.connect(lambda : self.__applyButton.setEnabled(True))
        edit._value_ = lambda e=edit : e.text()
        edit._key = definition["key"]
        return (edit,definition["label"])


    def __buildText_(
        self
        ,definition
        ,properties
        ):
        """
        Getter method.

        Parameters
        ----------
        definition : dictionary
                     The edit definition used to build the returned edit widget.
        properties : dictionary
                     The properties of the block used to get the initial value
                     of the returned edit widget.

        Returns
        -------
        ret0 : socref.gui.PlainTextEdit
               A new text editor configured for the given definition and
               properties.
        ret1 : string
               A label for adding it to a form.
        """
        edit = gui.PlainTextEdit(speller=definition.get("speller",False),popup=True)
        edit.setPlainText(properties[definition["key"]])
        edit.textChanged.connect(lambda : self.__applyButton.setEnabled(True))
        edit._value_ = lambda e=edit : e.toPlainText()
        edit._key = definition["key"]
        return (edit,definition["label"])


    def __setupGui_(
        self
        ):
        """
        Initializes the GUI of this new block edit dock.
        """
        self.__applyButton.clicked.connect(self.__apply_)
        self.__applyButton.setEnabled(False)
        bottom = qtw.QHBoxLayout()
        bottom.addWidget(self.__applyButton)
        bottom.addStretch()
        layout = qtw.QVBoxLayout()
        layout.addWidget(self.__area)
        layout.addLayout(bottom)
        central = qtw.QWidget()
        central.setLayout(layout)
        self.setWidget(central)


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot()
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
                ,core.Role.Properties
            )
            self.__applyButton.setEnabled(False)


    @qtc.Slot(qtc.QModelIndex)
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
            answer = qtw.QMessageBox.question(
                self
                ,"Unsaved Changes"
                ,"The current block has unsaved modifications. Discarding will cause modifications"
                 " to be lost!"
                ,qtw.QMessageBox.Save | qtw.QMessageBox.Discard
            )
            if answer == qtw.QMessageBox.Save:
                self.__apply_()
        self.__index = index
        if self.__area.widget():
            self.__area.widget().deleteLater()
            gc.collect()
        if index.isValid():
            self.__area.setWidget(self.__buildFormWidget_(index))
            self.__applyButton.setEnabled(False)
        else:
            self.__edits.clear()
            self.__area.setWidget(None)


    @qtc.Slot(qtc.QModelIndex)
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


    @qtc.Slot()
    def __indexRemoved_(
        self
        ):
        """
        Called to remove this dock's current index, setting it to invalid.
        """
        self.__index = qtc.QModelIndex()
