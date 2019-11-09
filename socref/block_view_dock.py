"""
todo
"""
from PySide2.QtCore import Qt, Slot as QtSlot, QModelIndex
from PySide2.QtWidgets import QDockWidget, QLabel
from .project_model import Project_Model






class Block_View_Dock(QDockWidget):


    def __init__(self, parent=None):
        QDockWidget.__init__(self,parent)
        #
        self.__view = None
        #:


    def set_view(self, view):
        self.__view = view
        self.__view.current_changed.connect(self.__current_changed_)


    @QtSlot(QModelIndex)
    def __current_changed_(self, index):
        if index.isValid() :
            model = self.__view.model()
            self.setWindowTitle(model.data(index,Qt.DisplayRole))
            label = QLabel(self)
            label.setAlignment(Qt.AlignTop)
            label.setWordWrap(True)
            label.setTextFormat(Qt.RichText)
            label.setContentsMargins(4,16,4,4)
            label.setText(model.data(index,Project_Model.VIEW_ROLE))
            self.setWidget(label)
        else :
            self.setWindowTitle("")
            self.setWidget(None)

