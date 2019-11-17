"""
todo
"""
from PySide2 import QtCore as qtc
from PySide2 import QtGui as qtg
from PySide2 import QtWidgets as qtw
from . import project_model as pm
from . import block_factory as bf






class Project_View(qtw.QTreeView):


    #
    current_changed = qtc.Signal(qtc.QModelIndex)


    def __init__(self,parent=None):
        qtw.QTreeView.__init__(self, parent)
        #
        self.__model = None
        #
        self.__add_actions = []
        #
        self.__undo_action = qtw.QAction("Undo",self)
        #
        self.__redo_action = qtw.QAction("Redo",self)
        #
        self.__remove_action = qtw.QAction("Remove",self)
        #
        self.__cut_action = qtw.QAction("Cut",self)
        #
        self.__copy_action = qtw.QAction("Copy",self)
        #
        self.__paste_action = qtw.QAction("Paste",self)
        #
        self.__insert_before_action = qtw.QAction("Before",self)
        #
        self.__insert_into_action = qtw.QAction("Into",self)
        #
        self.__insert_after_action = qtw.QAction("After",self)
        #
        self.__move_up_action = qtw.QAction("Move Up",self)
        #
        self.__move_down_action = qtw.QAction("Move Down",self)
        #
        self.__context_menu = qtw.QMenu("Edit",self)
        #
        self.__add_menu = qtw.QMenu("Add")
        #
        self.__insert = self.__INTO
        #:
        self.setSelectionMode(qtw.QAbstractItemView.ExtendedSelection)
        self.__setup_actions_()
        self.__setup_context_menu_()
        self.setContextMenuPolicy(qtc.Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.__context_menu_requested)


    def context_menu(self):
        return self.__context_menu


    def undo_action(self):
        return self.__undo_action


    def redo_action(self):
        return self.__redo_action


    def remove_action(self):
        return self.__remove_action


    def cut_action(self):
        return self.__cut_action


    def copy_action(self):
        return self.__copy_action


    def paste_action(self):
        return self.__paste_action


    def move_up_action(self):
        return self.__move_up_action


    def move_down_action(self):
        return self.__move_down_action


    def setModel(self, model):
        qtw.QTreeView.setModel(self,model)
        model.modelReset.connect(self.__model_reset_)
        model.destroyed.connect(self.__model_destroyed_)
        model.dataChanged.connect(self.__model_data_changed_)
        self.selectionModel().currentChanged.connect(self.__current_changed_)
        self.__model = model
        self.__update_context_menu_()


    @qtc.Slot()
    def __current_changed_(self,current,previous):
        self.__update_context_menu_()
        self.current_changed.emit(current)


    @qtc.Slot()
    def __model_reset_(self):
        self.__update_context_menu_()
        self.current_changed.emit(qtc.QModelIndex())


    @qtc.Slot()
    def __model_destroyed_(self):
        self.__model = None
        self.__update_context_menu_()
        self.current_changed.emit(qtc.QModelIndex())


    @qtc.Slot(qtc.QModelIndex, qtc.QModelIndex, list)
    def __model_data_changed_(self, top_left, bottom_right, roles):
        if top_left == self.selectionModel().currentIndex() :
            self.current_changed.emit(top_left)
            self.__update_context_menu_()


    @qtc.Slot()
    def __context_menu_requested(self, position):
        self.__context_menu.exec_(self.mapToGlobal(position))


    @qtc.Slot()
    def __undo_(self):
        if self.__model is not None :
            self.__model.undo()
            self.__update_context_menu_()


    @qtc.Slot()
    def __redo_(self):
        if self.__model is not None :
            self.__model.redo()
            self.__update_context_menu_()


    @qtc.Slot(str)
    def __add_(self, block_name):
        (row,parent) = self.__get_addition_values()
        if row is None : return
        self.__model.insertRows(row,(block_name,),parent)
        if not self.selectionModel().currentIndex().isValid() :
            self.selectionModel().setCurrentIndex(self.__model.index(0,0,qtc.QModelIndex())
                                                  ,qtc.QItemSelectionModel.Current)


    @qtc.Slot()
    def __remove_(self):
        if self.__model is not None :
            while self.selectionModel().hasSelection() :
                index = self.selectionModel().selectedIndexes()[0]
                parent = index.parent()
                self.__model.removeRow(index.row(),parent)


    @qtc.Slot()
    def __cut_(self):
        if self.__model is not None :
            self.__copy_()
            self.__remove_()


    @qtc.Slot()
    def __copy_(self):
        if self.__model is not None :
            indexes = self.selectionModel().selectedIndexes()
            if not indexes : return
            (Project_View.__xml_blocks
            ,Project_View.__block_names_set) = self.__model.copy_to_xml(indexes)
            self.__update_actions_()


    @qtc.Slot()
    def __paste_(self):
        if Project_View.__xml_blocks is None : return
        (row,parent) = self.__get_addition_values()
        if row is None : return
        self.__model.insert_rows_from_xml(row,Project_View.__xml_blocks,parent)


    @qtc.Slot()
    def __move_up_(self):
        if self.__model is not None :
            self.__model.move_row(-1,self.selectionModel().currentIndex())


    @qtc.Slot()
    def __move_down_(self):
        if self.__model is not None :
            self.__model.move_row(1,self.selectionModel().currentIndex())


    @qtc.Slot()
    def __insert_before_(self):
        self.__insert_before_action.setChecked(True)
        self.__insert_into_action.setChecked(False)
        self.__insert_after_action.setChecked(False)
        self.__insert = self.__BEFORE
        self.__update_context_menu_()


    @qtc.Slot()
    def __insert_into_(self):
        self.__insert_before_action.setChecked(False)
        self.__insert_into_action.setChecked(True)
        self.__insert_after_action.setChecked(False)
        self.__insert = self.__INTO
        self.__update_context_menu_()


    @qtc.Slot()
    def __insert_after_(self):
        self.__insert_before_action.setChecked(False)
        self.__insert_into_action.setChecked(False)
        self.__insert_after_action.setChecked(True)
        self.__insert = self.__AFTER
        self.__update_context_menu_()


    def __update_context_menu_(self):
        self.__update_actions_()
        self.__add_menu.clear()
        for action in self.__add_actions : self.__add_menu.addAction(action)
        self.__add_menu.setDisabled(self.__add_menu.isEmpty())


    def __update_actions_(self):
        self.__update_add_actions_()
        selection_model = self.selectionModel()
        selected = False if selection_model is None else selection_model.currentIndex().isValid()
        self.__remove_action.setDisabled(not selected)
        self.__cut_action.setDisabled(not selected)
        self.__copy_action.setDisabled(not selected)
        self.__paste_action.setDisabled(not self.__can_paste())
        self.__move_up_action.setDisabled(not selected)
        self.__move_down_action.setDisabled(not selected)


    def __update_add_actions_(self):
        while self.__add_actions : self.__add_actions.pop().deleteLater()
        if self.__model is None : return
        (row,index) = self.__get_addition_values()
        if index is None : return
        build_list = self.__model.data(index,pm.Project_Model.BUILD_LIST_ROLE)
        if build_list is not None :
            for block_name in build_list :
                action = qtw.QAction(block_name,self)
                action.setIcon(bf.Block_Factory().create(self.__model.lang_name(),block_name).icon())
                action.triggered.connect(lambda checked=False, name=block_name : self.__add_(name))
                self.__add_actions.append(action)


    def __can_paste(self):
        if self.__model is None or Project_View.__xml_blocks is None : return False
        (row,parent) = self.__get_addition_values()
        if parent is None : return False
        if not Project_View.__block_names_set & set(self.__model.data(parent,pm.Project_Model.BUILD_LIST_ROLE)) :
            return False
        return True


    def __get_addition_values(self):
        index = self.selectionModel().currentIndex()
        parent = None
        row = 1
        if self.__insert == self.__INTO :
            parent = index
            row = 0
        elif self.__insert == self.__BEFORE :
            if not index.isValid(): return (None,None)
            parent = index.parent()
            row = index.row()
        elif self.__insert == self.__AFTER :
            if not index.isValid(): return (None,None)
            parent = index.parent()
            row = index.row() + 1
        else: raise RuntimeError("Invalid addition given.")
        return (row,parent)


    def __setup_actions_(self):
        #
        action = self.__undo_action
        action.setIcon(qtg.QIcon.fromTheme("edit-undo"))
        action.setStatusTip("Undo the previous action.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Undo))
        action.triggered.connect(self.__undo_)
        self.addAction(action)
        #
        action = self.__redo_action
        action.setIcon(qtg.QIcon.fromTheme("edit-redo"))
        action.setStatusTip("Redo the previous undone action.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Redo))
        action.triggered.connect(self.__redo_)
        self.addAction(action)
        #
        action = self.__remove_action
        action.setIcon(qtg.QIcon.fromTheme("list-remove"))
        action.setStatusTip("Remove selected block(s).")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Delete))
        action.triggered.connect(self.__remove_)
        self.addAction(action)
        #
        action = self.__cut_action
        action.setIcon(qtg.QIcon.fromTheme("edit-cut"))
        action.setStatusTip("Cut selected block(s).")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Cut))
        action.triggered.connect(self.__cut_)
        self.addAction(action)
        #
        action = self.__copy_action
        action.setIcon(qtg.QIcon.fromTheme("edit-copy"))
        action.setStatusTip("Copy selected block(s).")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Copy))
        action.triggered.connect(self.__copy_)
        self.addAction(action)
        #
        action = self.__paste_action
        action.setIcon(qtg.QIcon.fromTheme("edit-paste"))
        action.setStatusTip("Paste before/into/after the current block, depending on the addition menu selection.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Paste))
        action.triggered.connect(self.__paste_)
        self.addAction(action)
        #
        action = self.__insert_before_action
        action.setStatusTip("Add or paste blocks before the current block.")
        action.setCheckable(True)
        action.setChecked(self.__insert == self.__BEFORE)
        action.triggered.connect(self.__insert_before_)
        #
        action = self.__insert_into_action
        action.setStatusTip("Add or paste blocks into the current block.")
        action.setCheckable(True)
        action.setChecked(self.__insert == self.__INTO)
        action.triggered.connect(self.__insert_into_)
        #
        action = self.__insert_after_action
        action.setStatusTip("Add or paste blocks after the current block.")
        action.setCheckable(True)
        action.setChecked(self.__insert == self.__AFTER)
        action.triggered.connect(self.__insert_after_)
        #
        action = self.__move_up_action
        action.setIcon(qtg.QIcon.fromTheme("go-up"))
        action.setStatusTip("Move current block up by one.")
        action.setShortcut(qtg.QKeySequence(qtc.Qt.CTRL + qtc.Qt.Key_Up))
        action.triggered.connect(self.__move_up_)
        self.addAction(action)
        #
        action = self.__move_down_action
        action.setIcon(qtg.QIcon.fromTheme("go-down"))
        action.setStatusTip("Move current block down by one.")
        action.setShortcut(qtg.QKeySequence(qtc.Qt.CTRL + qtc.Qt.Key_Down))
        action.triggered.connect(self.__move_down_)
        self.addAction(action)


    def __setup_context_menu_(self):
        menu = self.__context_menu
        menu.addAction(self.__undo_action)
        menu.addAction(self.__redo_action)
        menu.addSeparator()
        menu.addMenu(self.__add_menu)
        menu.addAction(self.__remove_action)
        menu.addSeparator()
        menu.addAction(self.__cut_action)
        menu.addAction(self.__copy_action)
        menu.addAction(self.__paste_action)
        menu.addSeparator()
        menu.addAction(self.__move_up_action)
        menu.addAction(self.__move_down_action)
        menu.addSeparator()
        additionMenu = menu.addMenu("Insert")
        additionMenu.addAction(self.__insert_before_action)
        additionMenu.addAction(self.__insert_into_action)
        additionMenu.addAction(self.__insert_after_action)
        self.__update_context_menu_()


    #
    __xml_blocks = None
    #
    __block_names_set = None
    #
    __BEFORE = 0
    #
    __INTO = 1
    #
    __AFTER = 2
