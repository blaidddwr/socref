"""
todo
"""
from PySide2.QtCore import Qt, QModelIndex
from PySide2.QtWidgets import QMainWindow
from .project_model import Project_Model
from .project_view import Project_View
from .block_view_dock import Block_View_Dock
from .block_edit_dock import Block_Edit_Dock






class Main_Window(QMainWindow):


    def __init__(self):
        QMainWindow.__init__(self)
        #
        self.__model = Project_Model(self)
        #
        self.__view = Project_View(self)
        #
        self.__block_view_dock = Block_View_Dock(self)
        #
        self.__block_edit_dock = Block_Edit_Dock(self)
        #:
        self.__view.setModel(self.__model)
        self.__block_view_dock.set_view(self.__view)
        self.__block_edit_dock.set_view(self.__view)
        self.setCentralWidget(self.__view)
        self.addDockWidget(Qt.RightDockWidgetArea,self.__block_view_dock)
        self.addDockWidget(Qt.RightDockWidgetArea,self.__block_edit_dock)
        self.__model.new("Python")
        self.__model.insertRows(0,("Package","Package","Package","Package","Package"),QModelIndex())
        self.__instances.append(self)


    #
    __instances = []
