"""
todo
"""
from PySide2.QtCore import QModelIndex
from PySide2.QtWidgets import QMainWindow
from .project_model import Project_Model
from .project_view import Project_View






class Main_Window(QMainWindow):


    def __init__(self):
        QMainWindow.__init__(self)
        #
        self.__view = Project_View(self)
        #
        self.__model = Project_Model(self)
        #:
        self.__view.setModel(self.__model)
        self.setCentralWidget(self.__view)
        self.__model.new("Python")
        self.__model.insertRows(0,("Package","Package","Package","Package","Package"),QModelIndex())
        self.__instances.append(self)


    #
    __instances = []
