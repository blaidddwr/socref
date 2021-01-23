"""
Contains the SetCommand class.
"""




class SetCommand(AbstractCommand):
    """
    This is the set command class. It implements the abstract command class.
    This command sets the properties of a given qt model index in the given
    project model.
    """


    def __init__(
        self
        ,fromProperties
        ,toProperties
        ,index
        ,model
    ):
        """
        Initializes a new set command with the given from properties, to
        properties, index, and model.

        Parameters
        ----------
        fromProperties : dictionary
                         Block properties that are the current properties of the
                         given index.
        toProperties : dictionary
                       Block properties that the given index is set to.
        index : PySide2.QtCore.QModelIndex
                The index whose properties are changed.
        model : socref.model.ProjectModel
                The project model whose given index block properties are
                changed.
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
