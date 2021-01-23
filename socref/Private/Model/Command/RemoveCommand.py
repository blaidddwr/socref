"""
Contains the RemoveCommand class.
"""




class RemoveCommand(AbstractCommand):
    """
    This is the remove command class. It implements the abstract command class.
    This command removes the given count of indexes starting at the given row
    from the given parent index in the given project model. This class saves the
    list of removed blocks and exposes it as a protected function so the inverse
    command of insertion can reuse this class.
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
        parent : PySide2.QtCore.QModelIndex
                 The parent index whose child indexes are removed.
        model : socref.model.ProjectModel
                The project model whose given indexes are removed.
        """
        pass


    def redo(
        self
    ):
        """
        Implements the .command.Command interface.
        """
        pass


    def undo(
        self
    ):
        """
        Implements the .command.Command interface.
        """
        pass
