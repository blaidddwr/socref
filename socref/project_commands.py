"""
todo
"""
from PySide2.QtCore import QModelIndex
from PySide2.QtWidgets import QUndoCommand






class Command(QUndoCommand):


    def __init__(self,model):
        QUndoCommand.__init__(self)
        #
        self.__model = model
        #:


    def _get_index_(self,rows):
        index = QModelIndex()
        for row in rows:
            index = self.__model.index(row,0,index)
            if not index.isValid(): raise RuntimeError("Move command rows are invalid.")
        return index


    def _build_rows_(self,index):
        rows = []
        while index.isValid():
            rows.append(index.row())
            index = index.parent()
        rows.reverse()
        return rows






class Remove(Command):


    def __init__(self,row,count,parent,model):
        Command.__init__(self,model)
        #
        self._blocks = None
        #
        self.__model = model
        #
        self.__parent_rows = self._build_rows_(parent)
        #
        self.__row = row
        #
        self.__count = count
        #:


    def undo(self):
        if self._blocks is not None:
            self.__model._insert_rows_(self.__row
                                       ,self._blocks
                                       ,self._get_index_(self.__parent_rows))
            self._blocks = None


    def redo(self):
        if self._blocks is None:
            self._blocks = self.__model._remove_rows_(self.__row
                                                      ,self.__count
                                                      ,self._get_index_(self.__parent_rows))






class Insert(Remove):


    def __init__(self,row,blocks,parent,model):
        Remove.__init__(self,row,len(blocks),parent,model)
        #
        self._blocks = blocks
        #:


    def undo(self):
        Remove.redo(self)


    def redo(self):
        Remove.undo(self)






class Move(Command):


    def __init__(self,change,index,model):
        Command.__init__(self,model)
        #
        self.__model = model
        #
        self.__parent_rows = self._build_rows_(index.parent())
        #
        self.__from_row = index.row()
        #
        self.__to_row = index.row() + change
        #
        self.__change = change
        #:


    def undo(self):
        self.__model._move_row_(self.__to_row
                                ,self.__from_row
                                ,self._get_index_(self.__parent_rows))


    def redo(self):
        self.__model._move_row_(self.__from_row
                                ,self.__to_row
                                ,self._get_index_(self.__parent_rows))
