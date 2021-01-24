"""
Contains the MoveCommand class.
"""
from ..Abstract.AbstractCommand import *




class MoveCommand(AbstractCommand):
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
        Initializes a new move command with the given change, index, and project
        model.

        Parameters
        ----------
        change : int
                 The change from the old row to the new row of the moved index.
                 A negative change is up and positive down by the number given.
        index : PySide2.QtCore.QModelIndex
                The index in the given project model that is moved.
        model : socref.model.ProjectModel
                The project model whose given index is moved.
        """
        super().__init__(model)
        self.__parentRows = self._buildRows_(index.parent())
        self.__fromRow = index.row()
        self.__toRow = index.row()+change


    def redo(
        self
    ):
        """
        Implements the .command.Command interface.
        """
        self._model._moveRow_(
            self.__fromRow
            ,self.__toRow
            ,self._getIndex_(self.__parentRows)
        )


    def undo(
        self
    ):
        """
        Implements the .command.Command interface.
        """
        self._model._moveRow_(
            self.__toRow
            ,self.__fromRow
            ,self._getIndex_(self.__parentRows)
        )
