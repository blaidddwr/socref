"""
Contains the CommandBase class.
"""
from ..Abstract.AbstractCommand import AbstractCommand
from PySide6.QtCore import QModelIndex




class CommandBase(AbstractCommand):
    """
    This is the command base class. It provides additional utility from its
    inherited abstract command class. Because qt model indexes can become
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
                The project model that this new command acts upon.
        """
        super().__init__()
        self._model = model


    def _rows_(
        self
        ,index
    ):
        """
        Getter method.

        Parameters
        ----------
        index : QModelIndex
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
        result : QModelIndex
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
