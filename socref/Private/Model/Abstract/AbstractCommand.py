"""
Contains the AbstractCommand class.
"""
from PySide2.QtCore import QModelIndex
from abc import ABC
from abc import abstractmethod




class AbstractCommand(ABC):
    """
    This is the abstract command class. A command is an abstract interface
    designed to do a single action upon a project model. It provides helper
    methods for converting a qt model index into a list of rows and rows back to
    an index.

    A command is an abstract interface designed to do a single action upon a
    project model. It provides interfaces for undoing or redoing a specific
    command on the model. The model itself is provided as a protected object to
    use when implementing undo and redo actions.

    It provides helper methods for converting a qt model index into a list of
    rows and rows back to an index. Because qt model indexes can become invalid
    when the model changes, the list of rows for each command must be saved
    instead. These rows are converted back to a valid index upon execution of
    the undo or redo action. The rows in the list start from the root down one
    level into the model tree for each row. An empty list represents the root
    index.
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


    def _buildRows_(
        self
        ,index
    ):
        """
        Getter method.

        Parameters
        ----------
        index : PySide2.QtCore.QModelIndex
                An index of this command's project model that is returns as a
                list of rows.

        Returns
        -------
        ret0 : list
               Rows that represent the given qt model index of this command's
               project model. An empty list is the root index.
        """
        rows = []
        while index.isValid():
            rows.append(index.row())
            index = index.parent()
        rows.reverse()
        return rows


    def _getIndex_(
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
        ret0 : PySide2.QtCore.QModelIndex
               A valid index created from this command's project model with the
               given list of rows. In this context valid can also be an invalid
               null index that represents the root.
        """
        index = QModelIndex()
        for row in rows:
            index = self._model.index(row,0,index)
            if not index.isValid():
                raise RuntimeError("Rows are invalid.")
        return index
