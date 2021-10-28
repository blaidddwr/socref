"""
Contains the CommandBase class.
"""
from ..Abstract.AbstractCommand import AbstractCommand
from PySide6.QtCore import QModelIndex




class CommandBase(AbstractCommand):
    """
    This is the command base class. It provides additional utility from its
    inherited abstract command class. Because Qt model indexes can become
    invalid when the model changes, the list of rows for each command must be
    saved. These rows are converted back to a valid index upon execution of the
    undo or redo action. Helper methods for converting between a Qt model index
    and a list of rows are provided to facilitate these requirements. The rows
    in the list start from the root down one level into the model tree for each
    row. An empty list represents the root index.
    """


    def __init__(
        self
        ,model
    ):
        """
        Initialize this new command base instance with the given project model.

        Parameters
        ----------
        model : ProjectModel
                The project model.
        """
        super().__init__()
        self._model = model


    def _rows_(
        self
        ,index
    ):
        """
        Creates and returns a list of rows, starting at the root parent index,
        generated from the given Qt model index of this command's project model.
        If the returned list is empty then it represents the root index.

        Parameters
        ----------
        index : QModelIndex
                The Qt model index.

        Returns
        -------
        result : list
                 The row indexes.
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
        Creates and returns a Qt model index in this command's project model
        generated from the given list of row indexes starting at the root index.

        Parameters
        ----------
        rows : list
               The row indexes.

        Returns
        -------
        result : QModelIndex
                 The Qt model index.
        """
        index = QModelIndex()
        for row in rows:
            index = self._model.index(row,0,index)
            if not index.isValid():
                raise RuntimeError("Rows are invalid.")
        return index
