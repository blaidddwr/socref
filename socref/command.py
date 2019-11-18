"""
Contains all classes that represent a single command or action that can be done to a project model.
Each command has an undo and redo method that allows project models undo and redo actions given to
it by the user.
"""
from PySide2 import QtCore as qtc








class Command():
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Init/Delete #
    ########################


    def __init__(self, model):
        """
        Detailed description.

        model : Detailed description.
        """
        self._model = model


    #######################
    # PROTECTED - Methods #
    #######################


    def _get_index_(self, rows):
        """
        Detailed description.

        rows : Detailed description.

        return : Yes
        """
        index = qtc.QModelIndex()
        for row in rows :
            index = self._model.index(row,0,index)
            if not index.isValid() : raise RuntimeError("Rows are invalid.")
        return index


    def _build_rows_(self, index):
        """
        Detailed description.

        index : Detailed description.

        return : Yes
        """
        rows = []
        while index.isValid() :
            rows.append(index.row())
            index = index.parent()
        rows.reverse()
        return rows








class Set(Command):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Init/Delete #
    ########################


    def __init__(self, from_props, to_props, index, model):
        """
        Detailed description.

        from_props : Detailed description.

        to_props : Detailed description.

        index : Detailed description.

        model : Detailed description.
        """
        Command.__init__(self,model)
        self.__rows = self._build_rows_(index)
        self.__from_props = from_props
        self.__to_props = to_props


    ####################
    # PUBLIC - Methods #
    ####################


    def undo(self):
        """
        Detailed description.
        """
        self._model._set_props_(self._get_index_(self.__rows),self.__from_props)


    def redo(self):
        """
        Detailed description.
        """
        self._model._set_props_(self._get_index_(self.__rows),self.__to_props)








class Remove(Command):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Init/Delete #
    ########################


    def __init__(self, row, count, parent, model):
        """
        Detailed description.

        row : Detailed description.

        count : Detailed description.

        parent : Detailed description.

        model : Detailed description.
        """
        Command.__init__(self,model)
        self._blocks = None
        self.__parent_rows = self._build_rows_(parent)
        self.__row = row
        self.__count = count


    ####################
    # PUBLIC - Methods #
    ####################


    def undo(self):
        """
        Detailed description.
        """
        if self._blocks is not None :
            self._model._insert_rows_(self.__row
                                      ,self._blocks
                                      ,self._get_index_(self.__parent_rows))
            self._blocks = None


    def redo(self):
        """
        Detailed description.
        """
        if self._blocks is None :
            self._blocks = self._model._remove_rows_(self.__row
                                                     ,self.__count
                                                     ,self._get_index_(self.__parent_rows))








class Insert(Command):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Init/Delete #
    ########################


    def __init__(self, row, blocks, parent, model):
        """
        Detailed description.

        row : Detailed description.

        blocks : Detailed description.

        parent : Detailed description.

        model : Detailed description.
        """
        Remove.__init__(self,row,len(blocks),parent,model)
        self._blocks = blocks


    ####################
    # PUBLIC - Methods #
    ####################


    def undo(self):
        """
        Detailed description.
        """
        Remove.redo(self)


    def redo(self):
        """
        Detailed description.
        """
        Remove.undo(self)








class Move(Command):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Init/Delete #
    ########################


    def __init__(self, change, index, model):
        """
        Detailed description.

        change : Detailed description.

        index : Detailed description.

        model : Detailed description.
        """
        Command.__init__(self,model)
        self.__parent_rows = self._build_rows_(index.parent())
        self.__from_row = index.row()
        self.__to_row = index.row() + change
        self.__change = change


    ####################
    # PUBLIC - Methods #
    ####################


    def undo(self):
        """
        Detailed description.
        """
        self._model._move_row_(self.__to_row
                               ,self.__from_row
                               ,self._get_index_(self.__parent_rows))


    def redo(self):
        """
        Detailed description.
        """
        self._model._move_row_(self.__from_row
                               ,self.__to_row
                               ,self._get_index_(self.__parent_rows))
