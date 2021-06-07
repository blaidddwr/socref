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
        Initializes a new insert command with the given row, blocks to insert,
        parent index, and project model.

        Parameters
        ----------
        row : int
              The row where the given blocks are inserted.
        blocks : list
                 Blocks that are inserted into the given project model.
        parent : QModelIndex
                 The parent index where the given blocks are inserted as
                 children.
        model : ProjectModel
                The project model where the given blocks are inserted.
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
