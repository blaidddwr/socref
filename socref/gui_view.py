"""
Detailed description.
"""
from PySide2 import QtCore as qtc
from PySide2 import QtGui as qtg
from PySide2 import QtWidgets as qtw
from . import abstract
from . import model








class Block_Dock(qtw.QDockWidget):
    """
    This is the block view dock class. It inherits the qt dock widget. It attaches itself to a
    project view, providing a detailed view of the currently indexed block. It connects the
    appropriate signal to listen for the current index of the view to change. The detailed view is
    presented in a qt label as rich text.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, parent=None):
        """
        Initializes a new block view dock with the given optional parent.

        parent : The optional qt object parent of this new block view dock.
        """
        qtw.QDockWidget.__init__(self,parent)
        #
        # Initialize this dock's view to none and window title.
        #
        self.__view = None
        self.setWindowTitle("(View)")


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


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot(qtc.QModelIndex)
    def __current_changed_(self, index):
        """
        Called to update this dock's detailed view to the new block at the given index. If the given
        index is invalid then this dock returns its view to a null state.

        index : The index of the new block whose detailed view is displayed by this dock.
        """
        #
        # Check to see if the given index is valid.
        #
        if index.isValid() :
            #
            # Get this dock's view's model and update its window title.
            #
            m = self.__view.model()
            self.setWindowTitle("[%s] %s (View)" %
                                (m.data(index,model.Role.BLOCK_TYPE)
                                 ,m.data(index,qtc.Qt.DisplayRole)))
            #
            # Create and initialize a new qt label with its rich text set to the detailed view of
            # the new block.
            #
            label = qtw.QLabel(self)
            label.setAlignment(qtc.Qt.AlignTop)
            label.setWordWrap(True)
            label.setTextFormat(qtc.Qt.RichText)
            label.setContentsMargins(4,16,4,4)
            label.setText(m.data(index,model.Role.VIEW))
            #
            # Set this dock's widget to the new qt label.
            #
            self.setWidget(label)
        #
        # Else the given index is invalid so update this dock's window title and set its widget to
        # nothing.
        #
        else:
            self.setWindowTitle("(View)")
            self.setWidget(None)








class Project(qtw.QTreeView):
    """
    This is the project view class. It inherits the qt tree view class. It provides the main tree
    view for a project model. Actions are provided for all basic editing actions done upon this
    view's model.

    It provides the main tree view for a project model. A view acts as the primary view for an
    entire project, allowing the user to navigate and view any part of the project tree. It provides
    a context menu that contains all actions a view can do upon its project model. The context menu
    provided fills the role of a main window's edit menu.

    Actions are provided for all basic editing actions done upon a view's model. These actions allow
    the user to edit the project tree with undo, redo, add, remove, cut, copy, paste, move up, and
    move down actions. For adding and pasting into the project an option is provided to insert
    before, into, or after the currently selected block. The individual qt actions are also provided
    if they need to be added to another custom menu or toolbar. The provided actions are enabled or
    disabled based off the current state of a view.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, parent=None):
        """
        Initializes a new project view with the given optional parent.

        parent : The optional qt object parent of this new project.
        """
        qtw.QTreeView.__init__(self, parent)
        #
        # Initialize this project's model to none.
        #
        self.__model = None
        #
        # Initialize this project's empty list of add actions.
        #
        self.__add_actions = []
        #
        # Initialize the rest of this project's actions.
        #
        self.__undo_action = qtw.QAction("Undo",self)
        self.__redo_action = qtw.QAction("Redo",self)
        self.__remove_action = qtw.QAction("Remove",self)
        self.__cut_action = qtw.QAction("Cut",self)
        self.__copy_action = qtw.QAction("Copy",self)
        self.__paste_action = qtw.QAction("Paste",self)
        self.__insert_before_action = qtw.QAction("Before",self)
        self.__insert_into_action = qtw.QAction("Into",self)
        self.__insert_after_action = qtw.QAction("After",self)
        self.__move_up_action = qtw.QAction("Move Up",self)
        self.__move_down_action = qtw.QAction("Move Down",self)
        #
        # Initialize this project's context and add menus.
        #
        self.__context_menu = qtw.QMenu("Edit",self)
        self.__add_menu = qtw.QMenu("Add")
        #
        # Initialize this project's insertion as into.
        #
        self.__insert = self.__INTO
        #
        # Setup the actions and menus of this project.
        #
        self.__setup_actions_()
        self.__setup_context_menu_()
        #
        # Initialize the inherited qt tree view of this project.
        #
        self.setSelectionMode(qtw.QAbstractItemView.ExtendedSelection)
        self.setContextMenuPolicy(qtc.Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.__context_menu_requested_)


    ####################
    # PUBLIC - Methods #
    ####################


    def context_menu(self):
        """
        Getter method.

        return : The context menu of this project.
        """
        return self.__context_menu


    def undo_action(self):
        """
        Getter method.

        return : The undo action of this project.
        """
        return self.__undo_action


    def redo_action(self):
        """
        Getter method.

        return : The redo action of this project.
        """
        return self.__redo_action


    def remove_action(self):
        """
        Getter method.

        return : The remove action of this project.
        """
        return self.__remove_action


    def cut_action(self):
        """
        Getter method.

        return : The cut action of this project.
        """
        return self.__cut_action


    def copy_action(self):
        """
        Getter method.

        return : The copy action of this project.
        """
        return self.__copy_action


    def paste_action(self):
        """
        Getter method.

        return : The paste action of this project.
        """
        return self.__paste_action


    def move_up_action(self):
        """
        Getter method.

        return : The move up action of this project.
        """
        return self.__move_up_action


    def move_down_action(self):
        """
        Getter method.

        return : The move down action of this project.
        """
        return self.__move_down_action


    def setModel(self, model):
        """
        Sets this project's model to the one given.

        model : The new model this project view displays and edits.
        """
        #
        # Set this project's inherited qt tree view model to the one given.
        #
        qtw.QTreeView.setModel(self,model)
        #
        # Connect all relevant signals to this project.
        #
        model.modelReset.connect(self.__model_reset_)
        model.destroyed.connect(self.__model_destroyed_)
        model.dataChanged.connect(self.__model_data_changed_)
        #
        # Connect all relevant selection model signals to this project. The selection model is
        # destroyed each time a new model is set in a qt tree view.
        #
        self.selectionModel().currentChanged.connect(self.__current_changed_)
        #
        # Set this project's model to the one given.
        #
        self.__model = model
        #
        # Update this project's context menu.
        #
        self.__update_context_menu_()


    ####################
    # PUBLIC - Signals #
    ####################


    #
    # Signals that the current index of this project's model's selection model has changed.
    #
    current_changed = qtc.Signal(qtc.QModelIndex)


    #####################
    # PRIVATE - Methods #
    #####################


    def __update_insert_(self, option):
        """
        Updates this project's insert option to the one given, setting all insert actions checked
        states appropriately.

        option : The new insert option set for this project.
        """
        #
        # Set the checked states of all three insert option actions to the new option state of this
        # project.
        #
        self.__insert_before_action.setChecked(option = self.__BEFORE)
        self.__insert_into_action.setChecked(option = self.__INTO)
        self.__insert_after_action.setChecked(option = self.__AFTER)
        #
        # Set this project's insert option to the new value and then update its context menu.
        #
        self.__insert = option
        self.__update_context_menu_()


    def __update_context_menu_(self):
        """
        Updates this project's context menu with the new selection state of its model. Updating
        includes enabling or disabling appropriate actions and rebuilding the add block menu.
        """
        #
        # Update this project's actions.
        #
        self.__update_actions_()
        #
        # Clear this project's add menu and populate it with the new add actions list.
        #
        self.__add_menu.clear()
        for action in self.__add_actions : self.__add_menu.addAction(action)
        #
        # Disable this project's add menu if it is empty.
        #
        self.__add_menu.setDisabled(self.__add_menu.isEmpty())


    def __update_actions_(self):
        """
        Updates this project's actions. Updating includes rebuilding its add block actions and
        enabling or disabling appropriate actions based off its selection model state.
        """
        #
        # Update this project's add actions list.
        #
        self.__update_add_actions_()
        #
        # Determine if one or more indexes are selected by this project's selection model.
        #
        selection_model = self.selectionModel()
        selected = False if selection_model is None else selection_model.currentIndex().isValid()
        #
        # Disable this project's remove, cut, copy, move up, and move down actions if no index is
        # selected.
        #
        self.__remove_action.setDisabled(not selected)
        self.__cut_action.setDisabled(not selected)
        self.__copy_action.setDisabled(not selected)
        self.__move_up_action.setDisabled(not selected)
        self.__move_down_action.setDisabled(not selected)
        #
        # Disable this project's paste action if it cannot paste any of its copied blocks.
        #
        self.__paste_action.setDisabled(not self.__can_paste_())


    def __update_add_actions_(self):
        """
        Update this project's add actions list. Updating involves clearing the current list and
        building it again based off this project's selection model's current index.
        """
        #
        # Clear this project's add actions list, deleting each action it contained.
        #
        while self.__add_actions : self.__add_actions.pop().deleteLater()
        #
        # If this project has no model this is finished.
        #
        if self.__model is None : return
        #
        # Get the insert values for adding new blocks, making sure the returned parent index exists.
        #
        (row,index) = self.__insert_values_()
        if index is None : return
        #
        # Get the build list of the parent index, making sure it is not none.
        #
        build_list = self.__model.data(index,model.Role.BUILD_LIST)
        if build_list is not None :
            #
            # Iterate through all block types of the build list.
            #
            for block_type in build_list :
                #
                # Create a new add action for the block type, fetching its icon and setting it to
                # the action, and then appending it to this project's add action list.
                #
                action = qtw.QAction(block_type,self)
                action.setIcon(abstract.Factory().create(self.__model.lang_name(),block_type).icon())
                action.triggered.connect(lambda checked=False, name=block_type : self.__add_(name))
                self.__add_actions.append(action)


    def __can_paste_(self):
        """
        Getter method.

        return : True if any of this project's global copied blocks can be pasted into its model at
                 the current index with the current insertion type. False if no copied block can be
                 pasted.
        """
        #
        # If this project has no model or no global copied blocks then return false.
        #
        if self.__model is None or Project.__xml_blocks is None : return False
        #
        # Get the insert values for pasting blocks, returning false if the returned index does not
        # exist.
        #
        (row,parent) = self.__insert_values_()
        if parent is None : return False
        #
        # If there is not a single global copied block type that is in the build list of the parent
        # index then return false.
        #
        if not Project.__block_types_set & set(self.__model.data(parent,model.Role.BUILD_LIST)) :
            return False
        #
        # All tests have passed so return true.
        #
        return True


    def __insert_values_(self):
        """
        Getter method.

        return : A tuple containing the row and parent index where blocks must be inserted into this
                 project's model by either adding new ones or pasting copied ones. If there is no
                 valid target for insertion the tuple returned is none and none.
        """
        #
        # Get the current index of this project's selection model.
        #
        index = self.selectionModel().currentIndex()
        #
        # Initialize the returned row and parent names to none.
        #
        parent = None
        row = None
        #
        # If this project's insert is into then set the parent and row accordingly.
        #
        if self.__insert == self.__INTO :
            parent = index
            row = 0
        #
        # Else if this project's insert is before or after then set the parent and row accordingly,
        # returning a none tuple if the current index is not valid.
        #
        elif self.__insert == self.__BEFORE :
            if not index.isValid(): return (None,None)
            parent = index.parent()
            row = index.row()
        elif self.__insert == self.__AFTER :
            if not index.isValid(): return (None,None)
            parent = index.parent()
            row = index.row() + 1
        #
        # Else this project's insert is invalid so raise an exception.
        #
        else: raise RuntimeError("Invalid addition given.")
        #
        # Return the determined row and parent for insertion.
        #
        return (row,parent)


    def __setup_actions_(self):
        """
        Initialize the qt actions of this new project view.
        """
        #
        # Initialize this project's undo action.
        #
        action = self.__undo_action
        action.setIcon(qtg.QIcon.fromTheme("edit-undo"))
        action.setStatusTip("Undo the previous action.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Undo))
        action.triggered.connect(self.__undo_)
        self.addAction(action)
        #
        # Initialize this project's redo action.
        #
        action = self.__redo_action
        action.setIcon(qtg.QIcon.fromTheme("edit-redo"))
        action.setStatusTip("Redo the previous undone action.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Redo))
        action.triggered.connect(self.__redo_)
        self.addAction(action)
        #
        # Initialize this project's remove action.
        #
        action = self.__remove_action
        action.setIcon(qtg.QIcon.fromTheme("list-remove"))
        action.setStatusTip("Remove selected block(s).")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Delete))
        action.triggered.connect(self.__remove_)
        self.addAction(action)
        #
        # Initialize this project's cut action.
        #
        action = self.__cut_action
        action.setIcon(qtg.QIcon.fromTheme("edit-cut"))
        action.setStatusTip("Cut selected block(s).")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Cut))
        action.triggered.connect(self.__cut_)
        self.addAction(action)
        #
        # Initialize this project's copy action.
        #
        action = self.__copy_action
        action.setIcon(qtg.QIcon.fromTheme("edit-copy"))
        action.setStatusTip("Copy selected block(s).")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Copy))
        action.triggered.connect(self.__copy_)
        self.addAction(action)
        #
        # Initialize this project's paste action.
        #
        action = self.__paste_action
        action.setIcon(qtg.QIcon.fromTheme("edit-paste"))
        action.setStatusTip("Paste before/into/after the current block, depending on the addition menu selection.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Paste))
        action.triggered.connect(self.__paste_)
        self.addAction(action)
        #
        # Initialize this project's move up action.
        #
        action = self.__move_up_action
        action.setIcon(qtg.QIcon.fromTheme("go-up"))
        action.setStatusTip("Move current block up by one.")
        action.setShortcut(qtg.QKeySequence(qtc.Qt.CTRL + qtc.Qt.Key_Up))
        action.triggered.connect(self.__move_up_)
        self.addAction(action)
        #
        # Initialize this project's move down action.
        #
        action = self.__move_down_action
        action.setIcon(qtg.QIcon.fromTheme("go-down"))
        action.setStatusTip("Move current block down by one.")
        action.setShortcut(qtg.QKeySequence(qtc.Qt.CTRL + qtc.Qt.Key_Down))
        action.triggered.connect(self.__move_down_)
        self.addAction(action)
        #
        # Initialize this project's insert before action.
        #
        action = self.__insert_before_action
        action.setStatusTip("Add or paste blocks before the current block.")
        action.setCheckable(True)
        action.setChecked(self.__insert == self.__BEFORE)
        action.triggered.connect(self.__insert_before_)
        #
        # Initialize this project's insert into action.
        #
        action = self.__insert_into_action
        action.setStatusTip("Add or paste blocks into the current block.")
        action.setCheckable(True)
        action.setChecked(self.__insert == self.__INTO)
        action.triggered.connect(self.__insert_into_)
        #
        # Initialize this project's insert after action.
        #
        action = self.__insert_after_action
        action.setStatusTip("Add or paste blocks after the current block.")
        action.setCheckable(True)
        action.setChecked(self.__insert == self.__AFTER)
        action.triggered.connect(self.__insert_after_)


    def __setup_context_menu_(self):
        """
        Initializes the context menu of this new project view.
        """
        #
        # Get this project's uninitialized context menu.
        #
        menu = self.__context_menu
        #
        # Add the undo and then redo actions followed by a separator.
        #
        menu.addAction(self.__undo_action)
        menu.addAction(self.__redo_action)
        menu.addSeparator()
        #
        # Add this project's add menu and then remove action followed by a separator.
        #
        menu.addMenu(self.__add_menu)
        menu.addAction(self.__remove_action)
        menu.addSeparator()
        #
        # Add cut, copy, and then paste actions followed by a separator.
        #
        menu.addAction(self.__cut_action)
        menu.addAction(self.__copy_action)
        menu.addAction(self.__paste_action)
        menu.addSeparator()
        #
        # Add the move up and then move down actions followed by a separator.
        #
        menu.addAction(self.__move_up_action)
        menu.addAction(self.__move_down_action)
        menu.addSeparator()
        #
        # Add a new insert sub menu. Add the insert before, insert into, and then insert after
        # actions to the insert sub menu.
        #
        additionMenu = menu.addMenu("Insert")
        additionMenu.addAction(self.__insert_before_action)
        additionMenu.addAction(self.__insert_into_action)
        additionMenu.addAction(self.__insert_after_action)
        #
        # Update this project's initialized context menu.
        #
        self.__update_context_menu_()


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot(qtc.QModelIndex,qtc.QModelIndex)
    def __current_changed_(self, current, previous):
        """
        Called to inform this project that its selection model's current index has changed to the
        one given from the previous one given.

        current : The new current index of this project's selection model.

        previous : The previous index of this project's selection model.
        """
        #
        # Update this project's context menu.
        #
        self.__update_context_menu_()
        #
        # Signal this project's current index has changed.
        #
        self.current_changed.emit(current)


    @qtc.Slot()
    def __model_reset_(self):
        """
        Called to inform this project that its model has reset itself.
        """
        #
        # Update this project's context menu.
        #
        self.__update_context_menu_()
        #
        # Signal this project's current index has changed to nothing.
        #
        self.current_changed.emit(qtc.QModelIndex())


    @qtc.Slot()
    def __model_destroyed_(self):
        """
        Called to inform this project that its model has been destroyed.
        """
        #
        # Clear this project's model reference and then update its context menu.
        #
        self.__model = None
        self.__update_context_menu_()
        #
        # Signal this project's current index has changed to nothing.
        #
        self.current_changed.emit(qtc.QModelIndex())


    @qtc.Slot(qtc.QModelIndex,qtc.QModelIndex,list)
    def __model_data_changed_(self, top_left, bottom_right, roles):
        """
        Called to inform this project that data has changed for the range of indexes from the given
        top left to the given bottom right with the given roles.

        top_left : The top left index in the range of indexes that has changed.

        bottom_right : The bottom right index in the range of indexes that has changed.

        roles : List of qt data roles that have changed in the given indexes.
        """
        #
        # If the given top left index is this project's selection model's current index then signal
        # that it has changed and update this project's context menu.
        #
        if top_left == self.selectionModel().currentIndex() :
            self.current_changed.emit(top_left)
            self.__update_context_menu_()


    @qtc.Slot(qtc.QPoint)
    def __context_menu_requested_(self, position):
        """
        Called to inform this project that its context menu has been requested by the GUI at the
        given relative point.

        position : The point where the context menu is requested relative to this project's widget.
        """
        self.__context_menu.exec_(self.mapToGlobal(position))


    @qtc.Slot()
    def __undo_(self):
        """
        Called to undo the last action done to this project's model.
        """
        #
        # Check if this project's model can undo an action.
        #
        if self.__model is not None :
            #
            # Undo the last action and then update this project's context menu.
            #
            self.__model.undo()
            self.__update_context_menu_()


    @qtc.Slot()
    def __redo_(self):
        """
        Called to redo the last undone action on this project's model.
        """
        #
        # Check if this project's model can redo an action.
        #
        if self.__model is not None :
            #
            # Redo the last action undone and then update this project's context menu.
            #
            self.__model.redo()
            self.__update_context_menu_()


    @qtc.Slot(str)
    def __add_(self, block_type):
        """
        Called to add a newly created block to this project's model at its current index using its
        selected insert option.

        block_type : The block type that is created and added to this project's model.
        """
        #
        # Get the row and parent index where the new block should be inserted, making sure it is
        # valid.
        #
        (row,parent) = self.__insert_values_()
        if row is None : return
        #
        # Insert a new block with the given type to this project's model at the insertion row and
        # parent.
        #
        self.__model.insertRows(row,(block_type,),parent)
        #
        # If the this project's selection model's current index is not valid then update it to the
        # newly created index. This is to fix a strange PySide2 bug.
        #
        if not self.selectionModel().currentIndex().isValid() :
            self.selectionModel().setCurrentIndex(self.__model.index(0,0,qtc.QModelIndex())
                                                  ,qtc.QItemSelectionModel.Current)


    @qtc.Slot()
    def __remove_(self):
        """
        Called to remove all selected indexes from this project's model.
        """
        #
        # Make sure this project has a model.
        #
        if self.__model is not None :
            #
            # Continue while this project's selection model has one or more indexes selected.
            #
            while self.selectionModel().hasSelection() :
                #
                # Get the first index that is selected and remove it from the model.
                #
                index = self.selectionModel().selectedIndexes()[0]
                parent = index.parent()
                self.__model.removeRow(index.row(),parent)


    @qtc.Slot()
    def __cut_(self):
        """
        Called to cut all selected indexes from this project's model.
        """
        #
        # Make sure this project has a model.
        #
        if self.__model is not None :
            #
            # Copy all selected indexes and then remove them.
            #
            self.__copy_()
            self.__remove_()


    @qtc.Slot()
    def __copy_(self):
        """
        Called to copy all selected indexes from this project's model.
        """
        #
        # Make sure this project has a model.
        #
        if self.__model is not None :
            #
            # Get this project's selection model's list of selected indexes, making sure it is not
            # empty.
            #
            indexes = self.selectionModel().selectedIndexes()
            if not indexes : return
            #
            # Copy the entire list of indexes to this project's global XML block copy and save all
            # block types copied to its global block type set.
            #
            (Project.__xml_blocks,Project.__block_types_set) = self.__model.copy_to_xml(indexes)
            #
            # Update this project's actions.
            #
            self.__update_actions_()


    @qtc.Slot()
    def __paste_(self):
        """
        Called to paste any globally copied blocks into this project's model at the current index
        using its selected insert option.
        """
        #
        # Make sure this project has a model and global has copied block XML data.
        #
        if self.__model is None or Project.__xml_blocks is None : return
        #
        # Get the row and parent index where the blocks should be pasted, making sure it is valid.
        #
        (row,parent) = self.__insert_values_()
        if row is None : return
        #
        # Paste the globally copied blocks into this project's model at the insertion row and
        # parent. The model will not insert any block whose type is not part of the parent's build
        # list.
        #
        self.__model.insert_from_xml(row,Project.__xml_blocks,parent)


    @qtc.Slot()
    def __move_up_(self):
        """
        Called to move the current index of this project's selection model up by one in its model.
        """
        if self.__model is not None :
            self.__model.move_row(-1,self.selectionModel().currentIndex())


    @qtc.Slot()
    def __move_down_(self):
        """
        Called to move the current index of this project's selection model down by one in its model.
        """
        if self.__model is not None :
            self.__model.move_row(1,self.selectionModel().currentIndex())


    @qtc.Slot()
    def __insert_before_(self):
        """
        Called to set this project's insert option to "before".
        """
        self.__update_insert_(self.__BEFORE)


    @qtc.Slot()
    def __insert_into_(self):
        """
        Called to set this project's insert option to "into".
        """
        self.__update_insert_(self.__INTO)


    @qtc.Slot()
    def __insert_after_(self):
        """
        Called to set this project's insert option to "after".
        """
        self.__update_insert_(self.__AFTER)


    ############################
    # PRIVATE - Static Objects #
    ############################


    #
    # Global XML byte array that stored the data of copied blocks, if any. If no blocks have been
    # copied then this is none.
    #
    __xml_blocks = None
    #
    # Global set that stores all unique block types that have been copied to the global XML data. If
    # no blocks have been copied then this is none. This is used to determine if the paste action
    # can be enabled.
    #
    __block_types_set = None


    #######################
    # PRIVATE - Constants #
    #######################


    #
    # The "before" insert option.
    #
    __BEFORE = 0
    #
    # The "into" insert option.
    #
    __INTO = 1
    #
    # The "after" insert option.
    #
    __AFTER = 2
