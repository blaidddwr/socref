"""
Contains the AbstractCommand class.
"""
from PySide2.QtCore import QModelIndex
from abc import ABC
from abc import abstractmethod




class AbstractCommand(ABC):
    """
    This is the abstract command class. A command is an action interface
    designed to redo or undo a single action upon a project model. Because qt
    model indexes can become invalid when the model changes, the list of rows
    for each command must be saved. These rows are converted back to a valid
    index upon execution of the undo or redo action. Helper methods for
    converting between a Qt model index and a list of rows are provided to
    facilitate these requirements. The rows in the list start from the root down
    one level into the model tree for each row. An empty list represents the
    root index.
    """


    def __init__(
        self
        ,model
    ):
        """
        Initialize a new command with the given project model.

        Parameters
        ----------
        model : socref.model.ProjectModel
                The project model that this new command acts upon.
        """
        super().__init__()
        self._model = model


    @abstractmethod
    def redo(
        self
    ):
        """
        This interface acts on this command's project model to redo its command.
        """
        pass


    @abstractmethod
    def undo(
        self
    ):
        """
        This interface acts on this command's project model to undo its command.
        """
        pass


    def _rows_(
        self
        ,index
    ):
        """
        Getter method.

        Parameters
        ----------
        index : PySide2.QtCore.QModelIndex
                An index of this command's project model.

        Returns
        -------
        result : list
                 Rows that represent the given qt model index of this command's
                 project model. An empty list is the root index.
        """
        rows = []
        while index.isValid():
            rows.append(index.row())
            index = index.parent()
        rows.reverse()
        return rows


    def _index_(
        self
        ,rows
    ):
        """
        Getter method.

        Parameters
        ----------
        rows : list
               Rows that represent a valid qt model index of this command's
               project model.

        Returns
        -------
        result : PySide2.QtCore.QModelIndex
                 A valid index created from this command's project model with
                 the given list of rows. An invalid null index is the root
                 index.
        """
        index = QModelIndex()
        for row in rows:
            index = self._model.index(row,0,index)
            if not index.isValid():
                raise RuntimeError("Rows are invalid.")
        return index
