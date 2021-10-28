"""
Contains the SetCommand class.
"""
from ..Base.CommandBase import CommandBase




class SetCommand(CommandBase):
    """
    This is the set command class. It implements the abstract command class.
    This command sets the properties of a given Qt model index in the given
    project model.
    """


    def __init__(
        self
        ,originalProperties
        ,newProperties
        ,index
        ,model
    ):
        """
        Initializes this new set command with the given original block
        properties, new block properties, Qt model index, and project model.
        This command's redo interface updates the block at the given Qt model
        index with the given new block properties. This command's undo interface
        updates the block at the given Qt model index with the given original
        block properties.

        Parameters
        ----------
        originalProperties : dictionary
                             The original block properties.
        newProperties : dictionary
                        The new block properties.
        index : QModelIndex
                The Qt model index.
        model : ProjectModel
                The project model.
        """
        super().__init__(model)
        self.__rows = self._rows_(index)
        self.__fromProperties = originalProperties
        self.__toProperties = newProperties


    def redo(
        self
    ):
        self._model._setProperties_(self._index_(self.__rows),self.__toProperties)


    def undo(
        self
    ):
        self._model._setProperties_(self._index_(self.__rows),self.__fromProperties)
