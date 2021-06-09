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
        index : QModelIndex
                The index whose properties are changed.
        model : ProjectModel
                The project model whose given index block properties are
                changed.
        """
        super().__init__(model)
        self.__rows = self._rows_(index)
        self.__fromProperties = fromProperties
        self.__toProperties = toProperties


    def redo(
        self
    ):
        self._model._setProperties_(self._index_(self.__rows),self.__toProperties)


    def undo(
        self
    ):
        self._model._setProperties_(self._index_(self.__rows),self.__fromProperties)
