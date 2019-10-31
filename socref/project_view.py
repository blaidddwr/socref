"""
todo
"""
from PySide2.QtCore import Qt,Signal as QtSignal,Slot as QtSlot,QModelIndex
from PySide2.QtGui import QKeySequence
from PySide2.QtWidgets import QAction,QMenu,QAbstractItemView,QTreeView






class Project_View(QTreeView):


    #
    current_changed = QtSignal(QModelIndex)


    def __init__(self,parent=None):
        QTreeView.__init__(self,parent)
        #
        self.__model = None
        #
        self.__remove_action = None
        #
        self.__cut_action = None
        #
        self.__copy_action = None
        #
        self.__paste_before_action = None
        #
        self.__paste_into_action = None
        #
        self.__paste_after_action = None
        #
        self.__move_up_action = None
        #
        self.__move_down_action = None
        #
        self.__add_actions = []
        #
        self.__context_menu = None
        #
        self.__add_menu = None
        #:
        self.setSelectionMode(QAbstractItemView.ExtendedSelection)
        self.__setup_actions_()
        self.__setup_context_menu_()
        self.setContextMenuPolicy(Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.__context_menu_requested)


    def context_menu(self):
        return __context_menu


    def setModel(self,model):
        QTreeView.setModel(self,model)
        model.modelReset.connect(self.__model_reset_)
        model.destroyed.connect(self.__model_destroyed_)
        model.dataChanged.connect(self.__model_data_changed_)
        self.selectionModel().currentChanged.connect(self.__current_changed_)
        self.__model = model
        self.__update_context_menu_()


    @QtSlot()
    def __current_changed_(self,current,previous):
        self.current_changed.emit(current)


    @QtSlot()
    def __model_reset_(self):
        self.__update_context_menu_()


    @QtSlot()
    def __model_destroyed_(self):
        self.__model = None
        self.__update_context_menu_()


    def __model_data_changed_(self,top_left,bottom_right,roles):
        if top_left == self.selectionModel().currentIndex(): self.__update_context_menu_()


    @QtSlot()
    def __context_menu_requested(self,position):
        self.__context_menu.exec_(self.mapToGlobal(position))


    @QtSlot()
    def __remove_(self):
        while self.selectionModel().hasSelection():
            index = self.selectionModel().selectedIndexes()[0]
            parent = self.__model.parent(index)
            self.__model.removeRow(index.row(),parent)


    @QtSlot()
    def __cut_(self):
        self.__copy_()
        self.__remove_()


    @QtSlot()
    def __copy_(self):
        indexes = self.selectionModel().selectedIndexes()
        if not indexes: return
        (Project_View.__xml_blocks
         ,Project_View.__block_names_set) = self.__model.copy_to_xml(indexes)


    @QtSlot()
    def __paste_before_(self):
        self.__paste_(self.__PASTE_BEFORE)


    @QtSlot()
    def __paste_into_(self):
        self.__paste_(self.__PASTE_INTO)


    @QtSlot()
    def __paste_after_(self):
        self.__paste_(self.__PASTE_AFTER)


    @QtSlot()
    def __move_up_(self):
        self.__model.move_row(-1,self.selectionModel().currentIndex())


    @QtSlot()
    def __move_down_(self):
        self.__model.move_row(2,self.selectionModel().currentIndex())


    def __paste_(self,type_):
        index = self.selectionModel().currentIndex()
        if not index.isValid(): return
        if Project_View.__xml_blocks is None: return
        parent = None
        row = 1
        if type_ == self.__PASTE_INTO:
            parent = index
            row = 0
        elif type_ == self.__PASTE_BEFORE:
            parent = self.__model.parent(index)
            row = index.row()
        elif type_ == self.__PASTE_AFTER:
            parent = self.__model.parent(index)
            row = index.row() + 1
        else: raise RuntimeError("Invalid type given.")
        self.__model.insert_rows_from_xml(row,Project_View.__xml_blocks,parent)


    def __update_context_menu_(self):
        self.__update_actions_()
        self.__add_menu.clear()
        for action in self.__add_actions: self.__add_menu.addAction(action)
        self.__add_menu.setDisabled(self.__add_menu.isEmpty())


    def __update_actions_(self):
        pass


    def __update_add_actions_(self):
        pass


    def __setup_actions_(self):
        self.__remove_action = self.__setup_action_("&Remove"
                                                    ,"Remove selected block(s)."
                                                    ,QKeySequence(QKeySequence.Delete)
                                                    ,self.__remove_)
        self.__cut_action = self.__setup_action_("&Cut"
                                                 ,"Cut selected block(s)."
                                                 ,QKeySequence(QKeySequence.Cut)
                                                 ,self.__cut_)
        self.__copy_action = self.__setup_action_("C&opy"
                                                  ,"Copy selected block(s)."
                                                  ,QKeySequence(QKeySequence.Copy)
                                                  ,self.__copy_)
        self.__paste_before_action = self.__setup_action_("Paste &Before"
                                                          ,"Paste before the current block."
                                                          ,QKeySequence(QKeySequence.Paste)
                                                          ,self.__paste_before_)
        self.__paste_into_action = self.__setup_action_("Paste &Into"
                                                        ,"Paste into the current block."
                                                        ,QKeySequence(Qt.CTRL + Qt.SHIFT + Qt.Key_V)
                                                        ,self.__paste_into_)
        self.__paste_after_action = self.__setup_action_("Paste &After"
                                                         ,"Paste after the current block."
                                                         ,QKeySequence(Qt.CTRL + Qt.ALT + Qt.Key_V)
                                                         ,self.__paste_after_)
        self.__move_up_action = self.__setup_action_("&Up"
                                                     ,"Move current block up by one."
                                                     ,QKeySequence(Qt.CTRL + Qt.Key_Up)
                                                     ,self.__move_up_)
        self.__move_down_action = self.__setup_action_("&Down"
                                                       ,"Move current block down by one."
                                                       ,QKeySequence(Qt.CTRL + Qt.Key_Down)
                                                       ,self.__move_down_)


    def __setup_action_(self,name,statusTip_,shortcut_,slot):
        action = QAction(name,self,statusTip=statusTip_,shortcut=shortcut_)
        action.triggered.connect(slot)
        self.addAction(action)
        return action


    def __setup_context_menu_(self):
        menu = self.__context_menu = QMenu("&Edit",self)
        self.__add_menu = menu.addMenu("&Add")
        menu.addAction(self.__remove_action)
        menu.addSeparator()
        menu.addAction(self.__cut_action)
        menu.addAction(self.__copy_action)
        menu.addAction(self.__paste_before_action)
        menu.addAction(self.__paste_into_action)
        menu.addAction(self.__paste_after_action)
        menu.addSeparator()
        menu.addAction(self.__move_up_action)
        menu.addAction(self.__move_down_action)
        self.__update_context_menu_()


    #
    __xml_blocks = None
    #
    __block_names_set = None
    #
    __PASTE_BEFORE = 0
    #
    __PASTE_INTO = 1
    #
    __PASTE_AFTER = 2
