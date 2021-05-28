"""
Contains the ProjectView class.
"""
from ...Factory import blockFactory
from ...Model.ProjectModel import *
from PySide6.QtCore import QItemSelectionModel
from PySide6.QtCore import QModelIndex
from PySide6.QtCore import QPoint
from PySide6.QtCore import Qt
from PySide6.QtCore import Signal
from PySide6.QtCore import Slot
from PySide6.QtGui import QAction
from PySide6.QtGui import QIcon
from PySide6.QtGui import QKeySequence
from PySide6.QtWidgets import QAbstractItemView
from PySide6.QtWidgets import QMenu
from PySide6.QtWidgets import QTreeView




class ProjectView(QTreeView):
    """
    This is the project view class. It provides the main tree view for a project
    model. A view acts as the primary view for an entire project, allowing the
    user to navigate and view any part of the project tree. It provides a
    context menu that contains all actions a view can do upon its project model.
    The context menu provided fills the role of a main window's edit menu.

    Actions are provided for all basic editing actions done upon a view's model.
    These actions allow the user to edit the project tree with undo, redo, add,
    remove, cut, copy, paste, move up, and move down actions. For adding and
    pasting into the project an option is provided to insert before, into, or
    after the currently selected block. The individual qt actions are also
    provided if they need to be added to another custom menu or toolbar. The
    provided actions are enabled or disabled based off the current state of a
    view.
    """
    __BEFORE = 0
    __INTO = 1
    __AFTER = 2
    __blockTypeSet = None
    __xmlBlocks = None


    #
    # Signals that the current index of this project has changed.
    #
    indexChanged = Signal(QModelIndex)


    #
    # Signals that the data of this project's current index has changed.
    #
    indexDataChanged = Signal()


    #
    # Signals that the currently indexed block of this project has moved to a
    # new index.
    #
    indexMoved = Signal(QModelIndex)


    #
    # Signals that the this project's current index is about to be removed from
    # its model.
    #
    indexRemoved = Signal()


    def __init__(
        self
        ,parent=None
    ):
        """
        Initializes a new project view with the given optional parent.

        Parameters
        ----------
        parent : object
                 Optional qt object parent of this new project.
        """
        super().__init__(parent)
        self.__model = None
        self.__addActions = []
        self.__undoAction = QAction("Undo",self)
        self.__redoAction = QAction("Redo",self)
        self.__removeAction = QAction("Remove",self)
        self.__cutAction = QAction("Cut",self)
        self.__copyAction = QAction("Copy",self)
        self.__pasteAction = QAction("Paste",self)
        self.__insertBeforeAction = QAction("Before",self)
        self.__insertIntoAction = QAction("Into",self)
        self.__insertAfterAction = QAction("After",self)
        self.__moveUpAction = QAction("Move Up",self)
        self.__moveDownAction = QAction("Move Down",self)
        self.__contextMenu = QMenu("Edit",self)
        self.__addMenu = QMenu("Add")
        self.__insert = self.__INTO
        self.__setupActions_()
        self.__setupContextMenu_()
        self.setSelectionMode(QAbstractItemView.ExtendedSelection)
        self.setContextMenuPolicy(Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.__contextMenuRequested_)


    def contextMenu(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : QMenu
                 The context menu of this project.
        """
        return self.__contextMenu


    def copyAction(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : QAction
                 The copy action of this project.
        """
        return self.__copyAction


    def cutAction(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : QAction
                 The cut action of this project.
        """
        return self.__cutAction


    def moveDownAction(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : QAction
                 The move down action of this project.
        """
        return self.__moveDownAction


    def moveUpAction(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : QAction
                 The move up action of this project.
        """
        return self.__moveUpAction


    def pasteAction(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : QAction
                 The paste action of this project.
        """
        return self.__pasteAction


    def redoAction(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : QAction
                 The redo action of this project.
        """
        return self.__redoAction


    def removeAction(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : QAction
                 The remove action of this project.
        """
        return self.__removeAction


    def setModel(
        self
        ,model
    ):
        """
        Sets this project's model to the one given.

        Parameters
        ----------
        model : ProjectModel
                The new model this project view displays and edits.
        """
        super().setModel(model)
        if self.__model:
            self.__model.modelReset.disconnect(self.__modelReset_)
            self.__model.destroyed.disconnect(self.__modelDestroyed_)
            self.__model.dataChanged.disconnect(self.__modelDataChanged_)
        model.modelReset.connect(self.__modelReset_)
        model.destroyed.connect(self.__modelDestroyed_)
        model.dataChanged.connect(self.__modelDataChanged_)
        self.selectionModel().currentChanged.connect(self.__currentChanged_)
        self.__model = model
        self.__updateContextMenu_()


    def undoAction(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : QAction
                 The undo action of this project.
        """
        return self.__undoAction


    @Slot(str)
    def __add_(
        self
        ,blockType
    ):
        """
        Called to add a newly created block to this project's model at its
        current index using its selected insert option.

        Parameters
        ----------
        blockType : string
                    The block type that is created and added to this project's
                    model.
        """
        (row,parent) = self.__insertValues_()
        if parent is None:
            return
        self.__model.insertRows(row,(blockType,),parent)


    def __canPaste_(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if any of this project's global copied blocks can be
                 pasted into its model at the current index with the current
                 insertion type. False if no copied block can be pasted.
        """
        if self.__model is None or not self.__model or ProjectView.__xmlBlocks is None:
            return False
        (row,parent) = self.__insertValues_()
        if parent is None:
            return False
        if not ProjectView.__blockTypeSet & set(self.__model.data(parent,Role.BuildList)):
            return False
        return True


    @Slot(QPoint)
    def __contextMenuRequested_(
        self
        ,position
    ):
        """
        Called to inform this project that its context menu has been requested
        by the GUI at the given relative point.

        Parameters
        ----------
        position : QPoint
                   The point where the context menu is requested relative to
                   this project's widget.
        """
        self.__contextMenu.exec(self.mapToGlobal(position))


    @Slot()
    def __copy_(
        self
    ):
        """
        Called to copy all selected indexes from this project's model.
        """
        if self.__model is not None:
            indexes = self.selectionModel().selectedIndexes()
            if not indexes:
                return
            (
                ProjectView.__xmlBlocks
                ,ProjectView.__blockTypeSet
            ) = (
                self.__model.copyToXml(indexes)
            )
            self.__updateActions_()


    @Slot(QModelIndex,QModelIndex)
    def __currentChanged_(
        self
        ,current
        ,previous
    ):
        """
        Called to inform this project that its selection model's current index
        has changed to the one given from the previous one given.

        Parameters
        ----------
        current : QModelIndex
                  The new current index of this project's selection model.
        previous : QModelIndex
                   The previous index of this project's selection model.
        """
        self.__updateContextMenu_()
        self.indexChanged.emit(current)


    @Slot()
    def __cut_(
        self
    ):
        """
        Called to cut all selected indexes from this project's model.
        """
        if self.__model is not None:
            self.__copy_()
            self.__remove_()


    @Slot()
    def __insertAfter_(
        self
    ):
        """
        Called to set this project's insert option to "after".
        """
        self.__updateInsert_(self.__AFTER)


    @Slot()
    def __insertBefore_(
        self
    ):
        """
        Called to set this project's insert option to "before".
        """
        self.__updateInsert_(self.__BEFORE)


    @Slot()
    def __insertInto_(
        self
    ):
        """
        Called to set this project's insert option to "into".
        """
        self.__updateInsert_(self.__INTO)


    def __insertValues_(
        self
    ):
        """
        Getter method.

        Returns
        -------
        row : int
              The row where blocks must be inserted into this project's model by
              either adding new ones or pasting copied ones. If there is no
              valid target for insertion then none is returned.
        index : QModelIndex
                The parent qt model index where blocks must be inserted into
                this project's model by either adding new ones or pasting copied
                ones. If there is no valid target for insertion then none is
                returned.
        """
        index = self.selectionModel().currentIndex()
        parent = None
        row = None
        if self.__insert == self.__INTO:
            parent = index
            row = 0
        elif self.__insert == self.__BEFORE:
            if not index.isValid(): return (None,None)
            parent = index.parent()
            row = index.row()
        elif self.__insert == self.__AFTER:
            if not index.isValid(): return (None,None)
            parent = index.parent()
            row = index.row() + 1
        else:
            raise RuntimeError("Invalid addition given.")
        return (row,parent)


    @Slot(QModelIndex,QModelIndex,list)
    def __modelDataChanged_(
        self
        ,topLeft
        ,bottomRight
        ,roles
    ):
        """
        Called to inform this project that data has changed for the range of
        indexes from the given top left to the given bottom right with the given
        roles.

        Parameters
        ----------
        topLeft : QModelIndex
                  The top left index in the range of indexes that has changed.
        bottomRight : QModelIndex
                      The bottom right index in the range of indexes that has
                      changed.
        roles : list
                Qt data roles that have changed in the given indexes.
        """
        if topLeft == self.selectionModel().currentIndex():
            self.indexDataChanged.emit()
            self.__updateContextMenu_()


    @Slot()
    def __modelDestroyed_(
        self
    ):
        """
        Called to inform this project that its model has been destroyed.
        """
        self.__model = None
        self.__updateContextMenu_()
        self.indexChanged.emit(QModelIndex())


    @Slot()
    def __modelReset_(
        self
    ):
        """
        Called to inform this project that its model has reset itself.
        """
        self.__updateContextMenu_()
        self.indexChanged.emit(QModelIndex())


    @Slot()
    def __moveDown_(
        self
    ):
        """
        Called to move the current index of this project's selection model down
        by one in its model.
        """
        if self.__model is not None:
            self.__model.moveRow(1,self.selectionModel().currentIndex())
            self.indexMoved.emit(self.selectionModel().currentIndex())


    @Slot()
    def __moveUp_(
        self
    ):
        """
        Called to move the current index of this project's selection model up by
        one in its model.
        """
        if self.__model is not None:
            self.__model.moveRow(-1,self.selectionModel().currentIndex())
            self.indexMoved.emit(self.selectionModel().currentIndex())


    @Slot()
    def __paste_(
        self
    ):
        """
        Called to paste any globally copied blocks into this project's model at
        the current index using its selected insert option.
        """
        if self.__model is None or ProjectView.__xmlBlocks is None:
            return
        (row,parent) = self.__insertValues_()
        if parent is None:
            return
        self.__model.insertFromXml(row,ProjectView.__xmlBlocks,parent)


    @Slot()
    def __redo_(
        self
    ):
        """
        Called to redo the last undone action on this project's model.
        """
        if self.__model is not None:
            self.__model.redo()
            self.__updateContextMenu_()


    @Slot()
    def __remove_(
        self
    ):
        """
        Called to remove all selected indexes from this project's model.
        """
        if self.__model is not None:
            while self.selectionModel().hasSelection():
                index = self.selectionModel().selectedIndexes()[0]
                if index == self.selectionModel().currentIndex():
                    self.indexRemoved.emit()
                parent = index.parent()
                self.__model.removeRow(index.row(),parent)


    def __setupActions_(
        self
    ):
        """
        Initialize the qt actions of this new project view.
        """
        action = self.__undoAction
        action.setIcon(QIcon.fromTheme("edit-undo"))
        action.setStatusTip("Undo the previous action.")
        action.setShortcut(QKeySequence(QKeySequence.Undo))
        action.triggered.connect(self.__undo_)
        self.addAction(action)
        action = self.__redoAction
        action.setIcon(QIcon.fromTheme("edit-redo"))
        action.setStatusTip("Redo the previous undone action.")
        action.setShortcut(QKeySequence(QKeySequence.Redo))
        action.triggered.connect(self.__redo_)
        self.addAction(action)
        action = self.__removeAction
        action.setIcon(QIcon.fromTheme("list-remove"))
        action.setStatusTip("Remove selected block(s).")
        action.setShortcut(QKeySequence(QKeySequence.Delete))
        action.triggered.connect(self.__remove_)
        self.addAction(action)
        action = self.__cutAction
        action.setIcon(QIcon.fromTheme("edit-cut"))
        action.setStatusTip("Cut selected block(s).")
        action.setShortcut(QKeySequence(QKeySequence.Cut))
        action.triggered.connect(self.__cut_)
        self.addAction(action)
        action = self.__copyAction
        action.setIcon(QIcon.fromTheme("edit-copy"))
        action.setStatusTip("Copy selected block(s).")
        action.setShortcut(QKeySequence(QKeySequence.Copy))
        action.triggered.connect(self.__copy_)
        self.addAction(action)
        action = self.__pasteAction
        action.setIcon(QIcon.fromTheme("edit-paste"))
        action.setStatusTip(
            "Paste before/into/after the current block, depending on the addition menu selection."
        )
        action.setShortcut(QKeySequence(QKeySequence.Paste))
        action.triggered.connect(self.__paste_)
        self.addAction(action)
        action = self.__moveUpAction
        action.setIcon(QIcon.fromTheme("go-up"))
        action.setStatusTip("Move current block up by one.")
        action.setShortcut(QKeySequence(Qt.CTRL + Qt.Key_Up))
        action.triggered.connect(self.__moveUp_)
        self.addAction(action)
        action = self.__moveDownAction
        action.setIcon(QIcon.fromTheme("go-down"))
        action.setStatusTip("Move current block down by one.")
        action.setShortcut(QKeySequence(Qt.CTRL + Qt.Key_Down))
        action.triggered.connect(self.__moveDown_)
        self.addAction(action)
        action = self.__insertBeforeAction
        action.setStatusTip("Add or paste blocks before the current block.")
        action.setCheckable(True)
        action.setChecked(self.__insert == self.__BEFORE)
        action.triggered.connect(self.__insertBefore_)
        action = self.__insertIntoAction
        action.setStatusTip("Add or paste blocks into the current block.")
        action.setCheckable(True)
        action.setChecked(self.__insert == self.__INTO)
        action.triggered.connect(self.__insertInto_)
        action = self.__insertAfterAction
        action.setStatusTip("Add or paste blocks after the current block.")
        action.setCheckable(True)
        action.setChecked(self.__insert == self.__AFTER)
        action.triggered.connect(self.__insertAfter_)


    def __setupContextMenu_(
        self
    ):
        """
        Initializes the context menu of this new project view.
        """
        menu = self.__contextMenu
        menu.addAction(self.__undoAction)
        menu.addAction(self.__redoAction)
        menu.addSeparator()
        menu.addMenu(self.__addMenu)
        menu.addAction(self.__removeAction)
        menu.addSeparator()
        menu.addAction(self.__cutAction)
        menu.addAction(self.__copyAction)
        menu.addAction(self.__pasteAction)
        menu.addSeparator()
        menu.addAction(self.__moveUpAction)
        menu.addAction(self.__moveDownAction)
        menu.addSeparator()
        insertMenu = menu.addMenu("Insert")
        insertMenu.addAction(self.__insertBeforeAction)
        insertMenu.addAction(self.__insertIntoAction)
        insertMenu.addAction(self.__insertAfterAction)
        self.__updateContextMenu_()


    @Slot()
    def __undo_(
        self
    ):
        """
        Called to undo the last action done to this project's model.
        """
        if self.__model is not None:
            self.__model.undo()
            self.__updateContextMenu_()


    def __updateActions_(
        self
    ):
        """
        Updates this project's actions. Updating includes rebuilding its add
        block actions and enabling or disabling appropriate actions based off
        its selection model state.
        """
        self.__updateAddActions_()
        selection_model = self.selectionModel()
        selected = False if selection_model is None else selection_model.currentIndex().isValid()
        self.__removeAction.setDisabled(not selected)
        self.__cutAction.setDisabled(not selected)
        self.__copyAction.setDisabled(not selected)
        self.__pasteAction.setDisabled(not self.__canPaste_())
        self.__moveUpAction.setDisabled(not selected)
        self.__moveDownAction.setDisabled(not selected)


    def __updateAddActions_(
        self
    ):
        """
        Update this project's add actions list. Updating involves clearing the
        current list and building it again based off this project's selection
        model's current index.
        """
        while self.__addActions:
            self.__addActions.pop().deleteLater()
        if self.__model is None:
            return
        (row,index) = self.__insertValues_()
        if index is None:
            return
        block_list = self.__model.data(index,Role.BuildList)
        if block_list is not None:
            for block_type in block_list:
                action = QAction(block_type.replace("_"," "),self)
                action.setIcon(
                    blockFactory.create(self.__model.langName(),block_type).icon()
                )
                action.triggered.connect(lambda checked=False, name=block_type : self.__add_(name))
                self.__addActions.append(action)


    def __updateContextMenu_(
        self
    ):
        """
        Updates this project's context menu with the new selection state of its
        model. Updating includes enabling or disabling appropriate actions and
        rebuilding the add block menu.
        """
        self.__updateActions_()
        self.__addMenu.clear()
        for action in self.__addActions:
            self.__addMenu.addAction(action)
        self.__addMenu.setDisabled(self.__addMenu.isEmpty())


    def __updateInsert_(
        self
        ,option
    ):
        """
        Updates this project's insert option to the one given, setting all
        insert actions checked states appropriately.

        Parameters
        ----------
        option : int
                 The new insert option set for this project.
        """
        self.__insertBeforeAction.setChecked(option == self.__BEFORE)
        self.__insertIntoAction.setChecked(option == self.__INTO)
        self.__insertAfterAction.setChecked(option == self.__AFTER)
        self.__insert = option
        self.__updateContextMenu_()


Role = ProjectModel.Role
