"""
Contains all GUI edit widgets that provide the user the ability to edit some form of data.
"""
import gc
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import settings
from . import model
from . import gui_util
from . import gui_dialog








class Plain_Text(qtw.QPlainTextEdit):
    """
    This is the plain text class. It inherits the qt plain text edit class. It provides additional
    functionality to its inherit class. Misspelled words are highlighted. A shortcut is provided to
    open a larger text editor dialog that has additional spell checking functionality. Misspelled
    word highlighting and the editor dialog popup features can be enabled or disabled.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, text="", parent=None, speller=False, popup=False):
        """
        Initializes a new plain text editor with the given optional text and parent. Spelling and
        dialog popup can also be enabled or disabled.

        text : The optional edited text of this new plain text editor.

        parent : The optional qt object parent of this new plain text editor.

        speller : True to enable misspelled word highlighting or false to disable it.

        popup : True to enable the popup edit dialog shortcut or false to disable it.
        """
        qtw.QPlainTextEdit.__init__(self,text,parent)
        #
        # Initialize the speller and popup indicators.
        #
        self.__speller = speller
        self.__popup = popup
        #
        # Initialize the speller if it is enabled, else keep this editor's highlighter as none.
        #
        self.__highlighter = None
        if speller : self.set_speller_enabled(True)
        #
        # Setup this editor's shortcut actions.
        #
        self.__setup_actions_()


    ####################
    # PUBLIC - Methods #
    ####################


    def set_speller_enabled(self, enabled):
        """
        Sets the state of this editor's spelling highlighter to enabled or disabled.

        enabled : True to enable this editor's spelling highlighter or false to disable it.
        """
        #
        # If highlighting is disabled then delete any existing speller highlighter of this editor.
        #
        if not enabled :
            if self.__highlighter is not None :
                self.__highlighter.deleteLater()
                self.__highlighter = None
        #
        # Else if it is enabled and there is no highlighter then create one for this editor.
        #
        elif self.__highlighter is None :
            self.__highlighter = gui_util.Spell_Highlighter(settings.DICTIONARY,self.document())
        #
        # Update the speller state.
        #
        self.__speller = enabled


    #####################
    # PRIVATE - Methods #
    #####################


    def __setup_actions_(self):
        """
        Initialize the qt action shortcuts of this new text editor.
        """
        #
        # Setup the popup text editor dialog action shortcut.
        #
        dialog = qtw.QAction(self)
        dialog.setShortcutContext(qtc.Qt.WidgetShortcut)
        dialog.setShortcut(qtc.Qt.CTRL + qtc.Qt.Key_E)
        dialog.triggered.connect(self.__dialog_)
        self.addAction(dialog)


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot()
    def __dialog_(self):
        """
        Called to open a model dialog text editor to edit this editor's text.
        """
        #
        # Make sure this editor's popup feature is enabled.
        #
        if self.__popup :
            #
            # Initialize a new text editor dialog.
            #
            dialog = gui_dialog.Text(self.toPlainText(),self,speller=self.__speller)
            dialog.setWindowTitle("Text Editor - Socrates' Reference")
            #
            # Modally execute the dialog, setting this editor's text to the dialog's text if it
            # returns with accepted value.
            #
            if dialog.exec_() : self.setPlainText(dialog.text())








class Block_Dock(qtw.QDockWidget):
    """
    This is the block edit dock class. It inherits the qt dock widget. It attaches itself to a
    project view. It provides a GUI edit form to the user for modifying a block's properties.

    It attaches itself to a project view. It connects the appropriate signal to listen for the
    current index of the view to change. When the index does change, it updates itself to reflect
    the new block that can be edited by the user.

    It provides a GUI edit form to the user for modifying a block's properties. The specific edit
    elements in the form is built based off the block's edit definitions, adding them all in the
    form in the same order as the edit definitions list. An apply button is also added to the end of
    the form to update the block's properties to the current values of all the edit widgets.
    Currently when the current index is changed this blindly wipes out any changes not applied to
    the block, this needs to be fixed.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, parent=None):
        """
        Initialize a new block edit dock with the given optional parent.

        parent : The optional qt object parent of this new block edit dock.
        """
        qtw.QDockWidget.__init__(self,parent)
        #
        # Initialize this dock's qt scroll area and apply button.
        #
        self.__area = qtw.QScrollArea(widgetResizable=True)
        self.__apply_button = qtw.QPushButton("Apply")
        #
        # Initialize this dock's current index to invalid, view to none, and edit widgets to empty.
        #
        self.__index = qtc.QModelIndex()
        self.__view = None
        self.__edits = []
        #
        # Initialize this dock's GUI.
        #
        self.__setup_gui_()


    ####################
    # PUBLIC - Methods #
    ####################


    def set_view(self, view):
        """
        Sets this dock's view to the one given. If this dock currently has a view it is disconnected
        from this dock.

        view : The new attached view of this dock.
        """
        #
        # If this dock already has a view then disconnect its signals.
        #
        if self.__view is not None :
            self.__view.current_changed.disconnect(self.__index_changed_)
            self.__view.current_changed.disconnect(self.__index_removed_)
        #
        # Update this dock's view to the one given.
        #
        self.__view = view
        #
        # Connect all index signals of the new view to this dock.
        #
        self.__view.index_changed.connect(self.__index_changed_)
        self.__view.index_removed.connect(self.__index_removed_)


    #####################
    # PRIVATE - Methods #
    #####################


    def __build_form_widget_(self, index):
        """
        Builds and initializes all edit widgets for all properties of the block at the given index
        based off its edit definitions, adding them to this dock's list of edit widgets. This clears
        any previous edit widgets from the last index that was built by this method.

        index : The index of the block whose property edit widgets are built.

        return : The form layout containing all built edit widgets and the apply button at the end.
        """
        #
        # Clear any existing edit widgets of this dock.
        #
        self.__edits.clear()
        try:
            #
            # Get the properties and edit definitions of the block at the given index.
            #
            props = self.__view.model().data(index,model.Role.PROPERTIES)
            defs = self.__view.model().data(index,model.Role.EDIT_DEFS)
            #
            # Initialize the qt form layout.
            #
            layout = qtw.QFormLayout()
            #
            # Iterate through all edit definitions.
            #
            for def_ in defs :
                #
                # Initialize the edit widget and label.
                #
                edit = None
                label = None
                #
                # Determine the edit definition type, calling the appropriate build method to create
                # a new edit widget for it. If the type is unknown then throw an exception.
                #
                if def_["type"] == "line" : (edit,label) = self.__build_line_(def_,props)
                elif def_["type"] == "text" : (edit,label) = self.__build_text_(def_,props)
                elif def_["type"] == "checkbox" : (edit,label) = self.__build_checkbox_(def_,props)
                elif def_["type"] == "combobox" : (edit,label) = self.__build_combobox_(def_,props)
                elif def_["type"] == "hidden" : edit = self.__build_hidden_(def_)
                else: raise RuntimeError("Unknown edit definition.")
                #
                # If the new edit widget is not hidden then add it to the form layout.
                #
                if label is not None : layout.addRow(label,edit)
                #
                # Append the edit widget to this dock's list.
                #
                self.__edits.append(edit)
            #
            # Initialize a new widget, setting its layout to the form layout.
            #
            ret = qtw.QWidget()
            ret.setContentsMargins(0,16,0,4)
            ret.setLayout(layout)
            #
            # Return the new widget.
            #
            return ret
        except:
            #
            # If any exception occurs during the build process then clear this dock's list of edit
            # widgets and raise it again.
            #
            self.__edits.clear()
            raise


    def __build_line_(self, definition, properties):
        """
        Getter method.

        definition : The edit definition used to build the returned edit widget.

        properties : The properties of the block used to get the initial value of the returned edit
                     widget.

        return : A tuple containing 2 items. The first is a new line edit widget configured for the
                 given definition and properties. The second is a string label for adding it to a
                 form.
        """
        edit = qtw.QLineEdit(properties[definition["key"]])
        edit.textChanged.connect(lambda : self.__apply_button.setEnabled(True))
        edit._value_ = lambda e=edit : e.text()
        edit._key = definition["key"]
        return (edit,definition["label"])


    def __build_text_(self, definition, properties):
        """
        Getter method.

        definition : The edit definition used to build the returned edit widget.

        properties : The properties of the block used to get the initial value of the returned edit
                     widget.

        return : A tuple containing 2 items. The first is a new plain text edit widget configured
                 for the given definition and properties. The second is a string label for adding it
                 to a form.
        """
        edit = Plain_Text(speller=definition.get("speller",False),popup=True)
        edit.setPlainText(properties[definition["key"]])
        edit.textChanged.connect(lambda : self.__apply_button.setEnabled(True))
        edit._value_ = lambda e=edit : e.toPlainText()
        edit._key = definition["key"]
        return (edit,definition["label"])


    def __build_checkbox_(self, definition, properties):
        """
        Getter method.

        definition : The edit definition used to build the returned edit widget.

        properties : The properties of the block used to get the initial value of the returned edit
                     widget.

        return : A tuple containing 2 items. The first is a new checkbox widget configured for the
                 given definition and properties. The second is a string label for adding it to a
                 form.
        """
        edit = qtw.QCheckBox(definition["label"])
        edit.setCheckState(qtc.Qt.Checked if int(properties[definition["key"]]) else qtc.Qt.Unchecked)
        edit.stateChanged.connect(lambda : self.__apply_button.setEnabled(True))
        edit._value_ = lambda e=edit : str(int(e.checkState() == qtc.Qt.Checked))
        edit._key = definition["key"]
        return (edit,"")


    def __build_combobox_(self, definition, properties):
        """
        Getter method.

        definition : The edit definition used to build the returned edit widget.

        properties : The properties of the block used to get the initial value of the returned edit
                     widget.

        return : A tuple containing 2 items. The first is a new combo box widget configured for the
                 given definition and properties. The second is a string label for adding it to a
                 form.
        """
        #
        # Create a new qt combo box.
        #
        edit = qtw.QComboBox()
        #
        # Iterate through all selections defined in the given edit definition, adding each one as an
        # item to the new combo box with or without an icon.
        #
        for selection in definition["selections"] :
            if "icon" in selection : edit.addItem(selection["icon"],selection["text"])
            else: edit.addItem(selection["text"])
        #
        # Initialize the new qt combo box using the given edit definition.
        #
        edit.setCurrentText(properties[definition["key"]])
        edit.currentTextChanged.connect(lambda : self.__apply_button.setEnabled(True))
        edit._value_ = lambda e=edit : e.currentText()
        edit._key = definition["key"]
        #
        # Return the combo box and label.
        #
        return (edit,definition["label"])


    def __build_hidden_(self, definition):
        """
        Getter method.

        definition : The edit definition used to build the returned edit widget.

        return : A new hidden edit widget configured for the given definition.
        """
        edit = qtw.QWidget()
        edit._value_ = lambda val=definition["value"] : val
        edit._key = definition["key"]
        return edit


    def __setup_gui_(self):
        """
        Initializes the GUI of this new block edit dock.
        """
        #
        # Configure this dock's apply button.
        #
        self.__apply_button.clicked.connect(self.__apply_)
        self.__apply_button.setEnabled(False)
        #
        # Create a new bottom horizontal box layout. Add this dock's apply button and then a
        # stretch.
        #
        bottom = qtw.QHBoxLayout()
        bottom.addWidget(self.__apply_button)
        bottom.addStretch()
        #
        # Create a new vertical box layout. Add this dock's scroll area and then the bottom layout.
        #
        layout = qtw.QVBoxLayout()
        layout.addWidget(self.__area)
        layout.addLayout(bottom)
        #
        # Create a generic widget, setting its layout and then setting it as this dock's widget.
        #
        central = qtw.QWidget()
        central.setLayout(layout)
        self.setWidget(central)


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot(qtc.QModelIndex)
    def __index_changed_(self, index):
        """
        Called to update the block this dock is editing to the new one at the given index. If the
        given index is invalid then this dock returns to a null state with no form.

        index : The index of the new block whose properties are edited by this dock.
        """
        #
        # Check if this dock's current index has unsaved changes.
        #
        if self.__index.isValid() and self.__apply_button.isEnabled() :
            #
            # Query the user about the unsaved changes, applying the changes to this dock's
            # currently indexed block if the user chooses to save the changes.
            #
            answer = qtw.QMessageBox.question(self
                                              ,"Unsaved Changes"
                                              ,"The current block has unsaved modifications."
                                               " Discarding will cause modifications to be lost!"
                                              ,qtw.QMessageBox.Save | qtw.QMessageBox.Discard)
            if answer == qtw.QMessageBox.Save : self.__apply_()
        self.__index = index
        #
        # If this dock has an old built edit widget form then delete it and force python garbage.
        # Garbage collection is required because rebuilding this dock's GUI causes large memory
        # leaks otherwise.
        #
        if self.__area.widget() :
            self.__area.widget().deleteLater()
            gc.collect()
        #
        # If the given index is valid then build a new form widget of edit widgets from the given
        # indexed block and disable the apply button.
        #
        if index.isValid() :
            self.__area.setWidget(self.__build_form_widget_(index))
            self.__apply_button.setEnabled(False)
        #
        # Else the given index is invalid so clear this dock's edit widgets.
        #
        else:
            self.__edits.clear()
            self.__area.setWidget(None)


    @qtc.Slot()
    def __index_removed_(self):
        """
        Called to remove this dock's current index, setting it to invalid.
        """
        self.__index = qtc.QModelIndex()


    @qtc.Slot()
    def __apply_(self):
        """
        Called to set this dock's currently indexed block's properties to the current values of its
        edit widgets. If the current index is not valid then this does nothing.
        """
        #
        # Make sure this dock's current index is valid.
        #
        if self.__index.isValid() :
            #
            # Set the properties of the current index to the values of this dock's edit widgets and
            # then disable the apply button.
            #
            self.__view.model().setData(self.__index
                                        ,{edit._key: edit._value_() for edit in self.__edits}
                                        ,model.Role.PROPERTIES)
            self.__apply_button.setEnabled(False)
