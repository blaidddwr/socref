"""
Contains all GUI edit widgets that provide the user the ability to edit some form of data.
"""
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import settings
from . import model
from . import gui_util
from . import gui_dialog








class Plain_Text(qtw.QPlainTextEdit):
    """
    This is the plain text class. It inherits the qt plain text edit class. It provides additional
    functionality to its inherit class.

    It provides additional functionality to its inherit class. Misspelled words are highlighted. A
    shortcut is provided to open a larger text editor dialog that has additional spell checking
    functionality. Misspelled word highlighting and the editor dialog popup features can be enabled
    or disabled.
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
        # Initialize this dock's view to none and edit widgets to empty.
        #
        self.__view = None
        self.__edits = []
        #
        # Initialize this dock's window title.
        #
        self.setWindowTitle("(Edit)")


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
        # If this dock already has a view then disconnect its signal.
        #
        if self.__view is not None : self.__view.current_changed.disconnect(self.__current_changed_)
        #
        # Update this docks view to the one given.
        #
        self.__view = view
        self.__view.current_changed.connect(self.__current_changed_)


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
                # Initialize the edit widget, label, and hidden names.
                #
                edit = None
                label = ""
                hidden = False
                #
                # If this is a line edit definition then initialize its edit widget and label.
                #
                if def_["type"] == "line" :
                    edit = qtw.QLineEdit(props[def_["key"]])
                    edit._value_ = lambda e=edit : e.text()
                    edit._key = def_["key"]
                    label = def_["label"]
                #
                # Else if this is a text edit definition then initialize its edit widget and label.
                #
                elif def_["type"] == "text" :
                    edit = Plain_Text(props[def_["key"]]
                                      ,speller=def_.get("speller",False)
                                      ,popup=True)
                    edit._value_ = lambda e=edit : e.toPlainText()
                    edit._key = def_["key"]
                    label = def_["label"]
                #
                # Else if this is a check box edit definition then initialize its edit widget and
                # leave the label blank.
                #
                elif def_["type"] == "checkbox" :
                    edit = qtw.QCheckBox(def_["label"])
                    edit.setCheckState(qtc.Qt.Checked if int(props[def_["key"]]) else qtc.Qt.Unchecked)
                    edit._value_ = lambda e=edit : str(int(e.checkState() == qtc.Qt.Checked))
                    edit._key = def_["key"]
                #
                # Else if this is a combo box edit definition then initialize its edit widget and
                # label.
                #
                elif def_["type"] == "combobox" :
                    edit = qtw.QComboBox()
                    for selection in def_["selections"] :
                        if "icon" in selection : edit.addItem(selection["icon"],selection["text"])
                        else: edit.addItem(selection["text"])
                    edit.setCurrentText(props[def_["key"]])
                    edit._value_ = lambda e=edit : e.currentText()
                    edit._key = def_["key"]
                    label = def_["label"]
                #
                # Else if this is a hidden edit definition then initialize its dummy edit widget and
                # set it as hidden.
                #
                elif def_["type"] == "hidden" :
                    edit = qtw.QWidget()
                    edit._value_ = lambda val=def_["value"] : val
                    edit._key = def_["key"]
                    hidden = True
                #
                # Else this is an invalid edit definition so throw an exception.
                #
                else: raise RuntimeError("Unknown edit definition.")
                #
                # If the new edit widget is not hidden then add it to the form layout.
                #
                if not hidden : layout.addRow(label,edit)
                #
                # Append the edit widget to this dock's list.
                #
                self.__edits.append(edit)
            #
            # Add a new apply button to the end of the form layout.
            #
            layout.addRow(self.__build_apply_())
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


    def __build_apply_(self):
        """
        Builder method.

        return : A qt layout containing a new apply button initialized to work this this dock' slot.
        """
        #
        # Initialize a new apply button for this dock.
        #
        button = qtw.QPushButton("Apply")
        button.clicked.connect(self.__apply_)
        #
        # Initialize a qt horizontal box layout. Add the new apply button and the and then a
        # stretch.
        #
        ret = qtw.QHBoxLayout()
        ret.addWidget(button)
        ret.addStretch()
        #
        # Return the box layout.
        #
        return ret


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot(qtc.QModelIndex)
    def __current_changed_(self, index):
        """
        Called to update the block this dock is editing to the new one at the given index. If the
        given index is invalid then this dock returns to a null state with no form.

        index : The index of the new block whose properties are edited by this dock.
        """
        #
        # If the given index is valid then update this dock's window title and build a new form of
        # edit widgets based off the given index.
        #
        if index.isValid() :
            m = self.__view.model()
            self.setWindowTitle("[%s] %s (Edit)" %
                                (m.data(index,model.Role.BLOCK_TYPE)
                                 ,m.data(index,qtc.Qt.DisplayRole)))
            self.setWidget(self.__build_form_widget_(index))
        #
        # Else the given index is invalid so clear this dock's window title and edit widgets.
        #
        else:
            self.setWindowTitle("(Edit)")
            self.__edits.clear()
            self.setWidget(None)


    @qtc.Slot()
    def __apply_(self):
        """
        Called to set this dock's view's currently indexed block's properties to the current values
        of its edit widgets. If the current index is not valid then this does nothing.
        """
        #
        # Get this dock's view's current index and make sure it is valid.
        #
        index = self.__view.selectionModel().currentIndex()
        if index.isValid() :
            #
            # Set the properties of the current index to the values of this dock's edit widgets.
            #
            self.__view.model().setData(index
                                        ,{edit._key: edit._value_() for edit in self.__edits}
                                        ,model.Role.PROPERTIES)
