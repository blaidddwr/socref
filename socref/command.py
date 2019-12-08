"""
Contains all classes that represent a single command or action that can be done to a project model.
"""
import abc
from PySide2 import QtCore as qtc








class Command(abc.ABC):
    """
    This is the abstract command class. A command is an abstract interface designed to do a single
    action upon a project model. It provides helper methods for converting a qt model index into a
    list of rows and rows back to an index.

    A command is an abstract interface designed to do a single action upon a project model. It
    provides interfaces for undoing or redoing a specific command on the model. The model itself is
    provided as a protected object to use when implementing undo and redo actions.

    It provides helper methods for converting a qt model index into a list of rows and rows back to
    an index. Because qt model indexes can become invalid when the model changes, the list of rows
    for each command must be saved instead. These rows are converted back to a valid index upon
    execution of the undo or redo action. The rows in the list start from the root down one level
    into the model tree for each row. An empty list represents the root index.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, model):
        """
        Initialize a new command with the given project model.

        model : The project model that this new command acts upon.
        """
        self._model = model


    #######################
    # PUBLIC - Interfaces #
    #######################


    @abc.abstractmethod
    def undo(self):
        """
        This interface acts on this command's project model to undo its command.
        """
        pass


    @abc.abstractmethod
    def redo(self):
        """
        This interface acts on this command's project model to redo its command.
        """
        pass


    #######################
    # PROTECTED - Methods #
    #######################


    def _get_index_(self, rows):
        """
        Getter method.

        rows : A list of rows that represent a valid qt model index of this command's project model.

        return : A valid qt model index created from this command's project model with the given
                 list of rows. In this context valid can also be an invalid null index that
                 represents the root.
        """
        index = qtc.QModelIndex()
        for row in rows:
            index = self._model.index(row,0,index)
            if not index.isValid():
                raise RuntimeError("Rows are invalid.")
        return index


    def _build_rows_(self, index):
        """
        Getter method.

        index : A qt model index of this command's project model that is returns as a list of rows.

        return : A list of rows that represent the given qt model index of this command's project
                 model. An empty list is the root index.
        """
        rows = []
        while index.isValid():
            rows.append(index.row())
            index = index.parent()
        rows.reverse()
        return rows








class SetCommand(Command):
    """
    This is the set command class. It implements the abstract command class. This command sets the
    properties of a given qt model index in the given project model.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, from_props, to_props, index, model):
        """
        Initializes a new set command with the given from properties, to properties, index, and
        model.

        from_props : A block properties dictionary that is the current properties of the given
                     index.

        to_props : A block properties dictionary that is the properties the given index is set to.

        index : The qt model index whose properties are changed.

        model : The project model whose given index block properties are changed.
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
        Implements the .command.Command interface.
        """
        self._model._set_properties_(self._get_index_(self.__rows),self.__from_props)


    def redo(self):
        """
        Implements the .command.Command interface.
        """
        self._model._set_properties_(self._get_index_(self.__rows),self.__to_props)








class RemoveCommand(Command):
    """
    This is the remove command class. It implements the abstract command class. This command removes
    the given count of indexes starting at the given row from the given parent index in the given
    project model. This class saves the list of removed blocks and exposes it as a protected
    function so the inverse command of insertion can reuse this class.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, row, count, parent, model):
        """
        Initializes a new remove command with the given row, count, parent index, and project model.

        row : The row of the first removed child of the given parent index.

        count : The number of children that are removed.

        parent : The parent index whose child indexes are removed.

        model : The project model whose given indexes are removed.
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
        Implements the .command.Command interface.
        """
        if self._blocks is not None:
            self._model._insert_rows_(
                self.__row
                ,self._blocks
                ,self._get_index_(self.__parent_rows)
            )
            self._blocks = None


    def redo(self):
        """
        Implements the .command.Command interface.
        """
        if self._blocks is None:
            self._blocks = self._model._remove_rows_(
                self.__row
                ,self.__count
                ,self._get_index_(self.__parent_rows)
            )








class InsertCommand(RemoveCommand):
    """
    This is the insert command class. It implements the abstract command class. It inherits from the
    remove command class because the insert command is simply the inverse of the remove command and
    therefore only needs to reverse the undo and redo commands.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, row, blocks, parent, model):
        """
        Initializes a new insert command with the given row, blocks to insert, parent index, and
        project model.

        row : The row where the given blocks are inserted.

        blocks : List of blocks that are inserted into the given project model.

        parent : The parent index where the given blocks are inserted as children.

        model : The project model where the given blocks are inserted.
        """
        Remove.__init__(self,row,len(blocks),parent,model)
        self._blocks = blocks


    ####################
    # PUBLIC - Methods #
    ####################


    def undo(self):
        """
        Implements the .command.Command interface.
        """
        Remove.redo(self)


    def redo(self):
        """
        Implements the .command.Command interface.
        """
        Remove.undo(self)








class MoveCommand(Command):
    """
    This is the move command class. It implements the abstract command class. This command moves the
    given from row to the given to row in the given parent index and project model.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, change, index, model):
        """
        Initializes a new move command with the given change, index, and project model.

        change : The change from the old row to the new row of the moved index. A negative change is
                 up and positive down by the number given.

        index : The index in the given project model that is moved.

        model : The project model whose given index is moved.
        """
        Command.__init__(self,model)
        self.__parent_rows = self._build_rows_(index.parent())
        self.__from_row = index.row()
        self.__to_row = index.row() + change


    ####################
    # PUBLIC - Methods #
    ####################


    def undo(self):
        """
        Implements the .command.Command interface.
        """
        self._model._move_row_(
            self.__to_row
            ,self.__from_row
            ,self._get_index_(self.__parent_rows)
        )


    def redo(self):
        """
        Implements the .command.Command interface.
        """
        self._model._move_row_(
            self.__from_row
            ,self.__to_row
            ,self._get_index_(self.__parent_rows)
        )
