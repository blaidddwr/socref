"""
Contains the BlockViewDock class.
"""
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import core








class BlockViewDock(qtw.QDockWidget):
    """
    This is the block view dock class. It attaches itself to a project view,
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
        self.__label = qtw.QLabel(
            alignment=qtc.Qt.AlignTop
            ,wordWrap=True
            ,textFormat=qtc.Qt.RichText
        )
        self.__view = None
        self.__setupGui_()


    def setView(
        self
        ,view
        ):
        """
        Sets this dock's view to the one given. If this dock currently has a
        view it is disconnected from this dock.

        Parameters
        ----------
        view : socref.gui.ProjectView
               The new attached view of this dock.
        """
        if self.__view is not None:
            self.__view.current_changed.disconnect(self.__indexChanged_)
        self.__view = view
        self.__view.indexChanged.connect(self.__indexChanged_)
        self.__view.indexDataChanged.connect(self.__indexChanged_)


    def __setupGui_(
        self
        ):
        """
        Initialize the GUI of this new block view dock.
        """
        self.__label.setContentsMargins(4,16,4,4)
        area = qtw.QScrollArea(widgetResizable=True)
        area.setWidget(self.__label)
        self.setWidget(area)


    @qtc.Slot(qtc.QModelIndex)
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
            self.__label.setText(self.__view.model().data(index,core.Role.View))
        else:
            self.__label.setText("")
