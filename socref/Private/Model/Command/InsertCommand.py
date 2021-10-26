"""
Contains the InsertCommand class.
"""
from .RemoveCommand import RemoveCommand




class InsertCommand(RemoveCommand):
    """
    This is the insert command class. It implements the abstract command class.
    It inherits from the remove command class because the insert command is
    simply the inverse of the remove command and therefore only needs to reverse
    the undo and redo commands.
    """


    def __init__(
        self
        ,row
        ,blocks
        ,parent
        ,model
    ):
        """
        Initializes this new insert command with the given row index, blocks,
        parent index, and project model. This command's redo inserts the blocks
        into the parent index at the row index.

        Parameters
        ----------
        row : int
              The row index.
        blocks : list
                 The blocks.
        parent : QModelIndex
                 The parent index.
        model : ProjectModel
                The project model.
        """
        super().__init__(row,len(blocks),parent,model)
        self._blocks = blocks


    def redo(
        self
    ):
        super().undo()


    def undo(
        self
    ):
        super().redo()
