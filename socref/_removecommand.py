"""
Contains the RemoveCommand class.
"""
from ._abstractcommand import AbstractCommand








class RemoveCommand(AbstractCommand):
    """
    This is the remove command class. It implements the abstract command class. This command removes
    the given count of indexes starting at the given row from the given parent index in the given
    project model. This class saves the list of removed blocks and exposes it as a protected
    function so the inverse command of insertion can reuse this class.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(
        self
        ,row
        ,count
        ,parent
        ,model
        ):
        """
        Initializes a new remove command with the given row, count, parent index, and project model.

        Parameters
        ----------
        row : int
              The row of the first removed child of the given parent index.
        count : int
                The number of children that are removed.
        parent : PySide2.QtCore.QModelIndex
                 The parent index whose child indexes are removed.
        model : socref.model.ProjectModel
                The project model whose given indexes are removed.
        """
        AbstractCommand.__init__(self,model)
        self._blocks = None
        self.__parentRows = self._buildRows_(parent)
        self.__row = row
        self.__count = count


    ####################
    # PUBLIC - Methods #
    ####################


    def redo(
        self
        ):
        """
        Implements the .command.Command interface.
        """
        if self._blocks is None:
            self._blocks = self._model._removeRows_(
                self.__row
                ,self.__count
                ,self._getIndex_(self.__parentRows)
            )


    def undo(
        self
        ):
        """
        Implements the .command.Command interface.
        """
        if self._blocks is not None:
            self._model._insertRows_(
                self.__row
                ,self._blocks
                ,self._getIndex_(self.__parentRows)
            )
            self._blocks = None
