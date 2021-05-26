"""
Contains the RemoveCommand class.
"""
from ..Base.CommandBase import *




class RemoveCommand(CommandBase):
    """
    This is the remove command class. It implements the abstract command class.
    This command removes the given count of indexes starting at the given row
    from the given parent index in the given project model. The removed blocks
    are saved as a protected attribute so the remove action can be undone. It is
    protected so the inverse insert command can use it.
    """


    def __init__(
        self
        ,row
        ,count
        ,parent
        ,model
    ):
        """
        Initializes a new remove command with the given row, count, parent
        index, and project model.

        Parameters
        ----------
        row : int
              The row of the first removed child of the given parent index.
        count : int
                The number of children that are removed.
        parent : QModelIndex
                 The parent index whose child indexes are removed.
        model : ProjectModel
                The project model whose given indexes are removed.
        """
        super().__init__(model)
        self._blocks = None
        self.__parentRows = self._rows_(parent)
        self.__row = row
        self.__count = count


    def redo(
        self
    ):
        if self._blocks is None:
            self._blocks = self._model._removeRows_(
                self.__row
                ,self.__count
                ,self._index_(self.__parentRows)
            )


    def undo(
        self
    ):
        if self._blocks is not None:
            self._model._insertRows_(
                self.__row
                ,self._blocks
                ,self._index_(self.__parentRows)
            )
            self._blocks = None
