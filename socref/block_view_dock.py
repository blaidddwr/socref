"""
todo
"""
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import project_model as pm






class Block_View_Dock(qtw.QDockWidget):


    def __init__(self, parent=None):
        qtw.QDockWidget.__init__(self,parent)
        #
        self.__view = None
        #:
        self.setWindowTitle("(View)")


    def set_view(self, view):
        self.__view = view
        self.__view.current_changed.connect(self.__current_changed_)


    @qtc.Slot(qtc.QModelIndex)
    def __current_changed_(self, index):
        if index.isValid() :
            model = self.__view.model()
            self.setWindowTitle("[%s] %s (View)" %
                                (model.data(index,pm.Project_Model.BLOCK_TYPE_ROLE)
                                 ,model.data(index,qtc.Qt.DisplayRole)))
            label = qtw.QLabel(self)
            label.setAlignment(qtc.Qt.AlignTop)
            label.setWordWrap(True)
            label.setTextFormat(qtc.Qt.RichText)
            label.setContentsMargins(4,16,4,4)
            label.setText(model.data(index,pm.Project_Model.VIEW_ROLE))
            self.setWidget(label)
        else :
            self.setWindowTitle("(View)")
            self.setWidget(None)
