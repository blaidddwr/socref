"""
Contains the MoveCommand class.
"""
from ..Base.CommandBase import CommandBase




class MoveCommand(CommandBase):
    """
    This is the move command class. It implements the abstract command class.
    This command moves the given from row to the given to row in the given
    parent index and project model.
    """


    def __init__(
        self
        ,change
        ,index
        ,model
    ):
        """
        Initializes this new move command with the given change, Qt model index,
        and project model. This command's redo interface changes the given
        index's row by the given change amount, negative being up and positive
        being down.

        Parameters
        ----------
        change : int
                 The change.
        index : QModelIndex
                The Qt model index.
        model : ProjectModel
                The project model.
        """
        super().__init__(model)
        self.__parentRows = self._rows_(index.parent())
        self.__fromRow = index.row()
        self.__toRow = index.row()+change


    def redo(
        self
    ):
        self._model._moveRow_(
            self.__fromRow
            ,self.__toRow
            ,self._index_(self.__parentRows)
        )


    def undo(
        self
    ):
        self._model._moveRow_(
            self.__toRow
            ,self.__fromRow
            ,self._index_(self.__parentRows)
        )
