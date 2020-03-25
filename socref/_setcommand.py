"""
Contains the SetCommand class.
"""
from ._abstractcommand import AbstractCommand








class SetCommand(AbstractCommand):
    """
    This is the set command class. It implements the abstract command class. This command sets the
    properties of a given qt model index in the given project model.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, fromProperties, toProperties, index, model):
        """
        Initializes a new set command with the given from properties, to properties, index, and
        model.

        Parameters
        ----------
        fromProperties : dictionary
                         Block properties that are the current properties of the given index.
        toProperties : dictionary
                       Block properties that the given index is set to.
        index : PySide2.QtCore.QModelIndex
                The index whose properties are changed.
        model : socref.model.ProjectModel
                The project model whose given index block properties are changed.
        """
        AbstractCommand.__init__(self,model)
        self.__rows = self._buildRows_(index)
        self.__fromProperties = fromProperties
        self.__toProperties = toProperties


    ####################
    # PUBLIC - Methods #
    ####################


    def redo(self):
        """
        Implements the .command.Command interface.
        """
        self._model._setProperties_(self._getIndex_(self.__rows),self.__toProperties)


    def undo(self):
        """
        Implements the .command.Command interface.
        """
        self._model._setProperties_(self._getIndex_(self.__rows),self.__fromProperties)
