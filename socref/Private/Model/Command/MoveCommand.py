"""
Contains the MoveCommand class.
"""




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
