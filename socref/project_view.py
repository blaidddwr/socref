"""
todo
"""
from PySide2.QtWidgets import QAbstractItemView,QTreeView






class Project_View(QTreeView):


    def __init__(self,parent=None):
        QTreeView.__init__(self,parent)
        #:
        self.setSelectionMode(QAbstractItemView.ExtendedSelection)
        self.setDragEnabled(True)
        self.viewport().setAcceptDrops(True)
        self.setDropIndicatorShown(True)


    def setModel(self,model):
        QTreeView.setModel(self,model)
        self.selectionModel().currentChanged.connect(self.current_changed)
        self.selectionModel().selectionChanged.connect(self.selection_changed)


    def current_changed(self,current,previous):
        print(f"current changed {current.row()}")


    def selection_changed(self,selected,deselected):
        print(f"selection changed {[index.row() for index in self.selectionModel().selectedIndexes()]}")
