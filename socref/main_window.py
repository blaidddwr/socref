"""
todo
"""
from PySide2.QtWidgets import QMainWindow,QTreeView
from .project_model import Project_Model




class Main_Window(QMainWindow):


    def __init__(self):
        QMainWindow.__init__(self)
        # 
        self.__model = Project_Model(self)
        #:
        #.
        tree_view = QTreeView(self)
        tree_view.setModel(self.__model)
        self.setCentralWidget(tree_view)
