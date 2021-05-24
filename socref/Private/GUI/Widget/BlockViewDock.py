"""
Contains the BlockViewDock class.
"""
from ...Model.ProjectModel import *
from PySide6.QtCore import QModelIndex
from PySide6.QtCore import Qt
from PySide6.QtCore import Slot
from PySide6.QtWidgets import QDockWidget
from PySide6.QtWidgets import QLabel
from PySide6.QtWidgets import QScrollArea




class BlockViewDock(QDockWidget):
    """
    This is the block view dock class. It interfaces itself to a project view,
    providing a detailed view of the currently indexed block. It connects the
    appropriate signal to listen for the current index of the view to change.
    The detailed view is presented in a qt label as rich text.
    """


    def __init__(
        self
        ,parent=None
    ):
        """
        Initializes a new block view dock with the given optional parent.

        Parameters
        ----------
        parent : object
                 Optional qt object parent of this new block view dock.
        """
        super().__init__(parent)
        self.__label = QLabel(
            alignment=Qt.AlignTop
            ,wordWrap=True
            ,textFormat=Qt.RichText
        )
        self.__view = None
        self.__setupGui_()


    def setView(
        self
        ,view
    ):
        """
        Sets this dock's project view to the one given. If this dock currently
        has a view it is disconnected from this dock.

        Parameters
        ----------
        view : socref.Private.GUI.Widget.ProjectView
               The newly interfaced project view of this dock. This project view
               is not made the child of this dock.
        """
        if self.__view is not None:
            self.__view.current_changed.disconnect(self.__indexChanged_)
        self.__view = view
        self.__view.indexChanged.connect(self.__indexChanged_)
        self.__view.indexDataChanged.connect(self.__indexChanged_)


    @Slot(QModelIndex)
    def __indexChanged_(
        self
    ):
        """
        Called to update this dock's detailed view to its view's currently
        indexed block. If its view's current index is invalid then this dock
        returns its view to a null state.
        """
        index = self.__view.selectionModel().currentIndex()
        if index.isValid():
            self.__label.setText(self.__view.model().data(index,Role.View))
        else:
            self.__label.setText("")


    def __setupGui_(
        self
    ):
        """
        Initialize the GUI of this new block view dock.
        """
        self.__label.setContentsMargins(4,16,4,4)
        area = QScrollArea(widgetResizable=True)
        area.setWidget(self.__label)
        self.setWidget(area)


Role = ProjectModel.Role
