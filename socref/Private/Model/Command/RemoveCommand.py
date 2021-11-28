"""
Contains the RemoveCommand class.
"""
from ..Base.CommandBase import CommandBase




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
        Initializes this new remove command with the given row, count, parent
        index, and project model. This command's redo interface removes the
        given count number of indexes from the given parent starting at the
        given row.

        Parameters
        ----------
        row : int
              The row.
        count : int
                The count.
        parent : QModelIndex
                 The parent index.
        model : ProjectModel
                The project model.
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
