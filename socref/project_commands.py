"""
todo
"""
from PySide2 import QtCore as qtc






class Command():


    def __init__(self,model):
        #
        self._model = model
        #:


    def _get_index_(self, rows):
        index = qtc.QModelIndex()
        for row in rows :
            index = self._model.index(row,0,index)
            if not index.isValid() : raise RuntimeError("Rows are invalid.")
        return index


    def _build_rows_(self, index):
        rows = []
        while index.isValid() :
            rows.append(index.row())
            index = index.parent()
        rows.reverse()
        return rows






class Set(Command):


    def __init__(self, from_props, to_props, index, model):
        Command.__init__(self,model)
        #
        self.__rows = self._build_rows_(index)
        #
        self.__from_props = from_props
        #
        self.__to_props = to_props


    def undo(self):
        self._model._set_props_(self._get_index_(self.__rows),self.__from_props)


    def redo(self):
        self._model._set_props_(self._get_index_(self.__rows),self.__to_props)






class Remove(Command):


    def __init__(self, row, count, parent, model):
        Command.__init__(self,model)
        #
        self._blocks = None
        #
        self.__parent_rows = self._build_rows_(parent)
        #
        self.__row = row
        #
        self.__count = count
        #:


    def undo(self):
        if self._blocks is not None :
            self._model._insert_rows_(self.__row
                                      ,self._blocks
                                      ,self._get_index_(self.__parent_rows))
            self._blocks = None


    def redo(self):
        if self._blocks is None :
            self._blocks = self._model._remove_rows_(self.__row
                                                     ,self.__count
                                                     ,self._get_index_(self.__parent_rows))






class Insert(Remove):


    def __init__(self, row, blocks, parent, model):
        Remove.__init__(self,row,len(blocks),parent,model)
        #
        self._blocks = blocks
        #:


    def undo(self):
        Remove.redo(self)


    def redo(self):
        Remove.undo(self)






class Move(Command):


    def __init__(self, change, index, model):
        Command.__init__(self,model)
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
        self._model._move_row_(self.__to_row
                               ,self.__from_row
                               ,self._get_index_(self.__parent_rows))


    def redo(self):
        self._model._move_row_(self.__from_row
                               ,self.__to_row
                               ,self._get_index_(self.__parent_rows))
