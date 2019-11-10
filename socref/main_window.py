"""
todo
"""
from PySide2.QtCore import Qt, Slot as QtSlot, QModelIndex
from PySide2.QtGui import QKeySequence
from PySide2.QtWidgets import QMainWindow, QAction, QMessageBox, QFileDialog
from .project_model import Project_Model
from .project_view import Project_View
from .block_view_dock import Block_View_Dock
from .block_edit_dock import Block_Edit_Dock
from .block_factory import Block_Factory






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
        #
        self.__open_action = QAction("Open",self)
        #
        self.__save_action = QAction("Save",self)
        #
        self.__save_as_action = QAction("Save As",self)
        #
        self.__close_action = QAction("Close",self)
        #
        self.__exit_action = QAction("Exit",self)
        #
        self.__new_actions = []
        #
        self.__path = None
        #:
        self.__instances.append(self)
        self.__model.modified.connect(self.__modified_)
        self.__view.setModel(self.__model)
        self.__block_view_dock.set_view(self.__view)
        self.__block_edit_dock.set_view(self.__view)
        self.setCentralWidget(self.__view)
        self.addDockWidget(Qt.RightDockWidgetArea,self.__block_view_dock)
        self.addDockWidget(Qt.RightDockWidgetArea,self.__block_edit_dock)
        self.__setup_actions_()
        self.__setup_menus_()
        self.__update_title_()
        self.__update_actions_()


    def closeEvent(self, event):
        if self.__is_ok_to_close_() : event.accept()
        else : event.ignore()


    def __update_actions_(self):
        self.__save_action.setDisabled(self.__path is None)
        self.__save_as_action.setDisabled(not self.__model)
        self.__close_action.setDisabled(not self.__model)


    def __is_ok_to_close_(self):
        if not self.__model or not self.__model.is_modified() : return True
        box = QMessageBox()
        box.setWindowTitle("Unsaved Project Changes")
        box.setText("The currently open project has unsaved changes. Closing the project will cause all unsaved changes to be lost!")
        box.setIcon(QMessageBox.Question)
        box.setStandardButtons(QMessageBox.Save | QMessageBox.Cancel | QMessageBox.Discard)
        result = box.exec_()
        if result == QMessageBox.Save :
            if self.__path is None : return self.__save_as_()
            else : return self.__save_()
        elif result == QMessageBox.Cancel : return False
        else : return True


    def __update_title_(self):
        if self.__model :
            self.setWindowTitle(f"{self.__model.name()}[*] ({self.__model.lang_name()}) - Socrates' Reference")
        else :
            self.setWindowTitle("Socrates' Reference")


    def __setup_actions_(self):
        self.__setup_new_actions_()
        self.__setup_file_actions_()


    def __setup_new_actions_(self):
        for lang in Block_Factory().langs() :
            self.__new_actions.append(QAction(lang,self))
            self.__new_actions[-1].triggered.connect(lambda checked=False,name=lang : self.__new_(name))
            self.addAction(self.__new_actions[-1])


    def __setup_file_actions_(self):
        #
        action = self.__open_action
        action.setStatusTip("Open an existing project.")
        action.setShortcut(QKeySequence(QKeySequence.Open))
        action.triggered.connect(self.__open_)
        self.addAction(action)
        #
        action = self.__save_action
        action.setStatusTip("Save the current project.")
        action.setShortcut(QKeySequence(QKeySequence.Save))
        action.triggered.connect(self.__save_)
        self.addAction(action)
        #
        action = self.__save_as_action
        action.setStatusTip("Save the current project to a provided file path.")
        action.setShortcut(QKeySequence(QKeySequence.SaveAs))
        action.triggered.connect(self.__save_as_)
        self.addAction(action)
        #
        action = self.__close_action
        action.setStatusTip("Close the current project.")
        action.setShortcut(QKeySequence(QKeySequence.Close))
        action.triggered.connect(self.__close_)
        self.addAction(action)
        #
        action = self.__exit_action
        action.setStatusTip("Exit this window.")
        action.setShortcut(QKeySequence(QKeySequence.Quit))
        action.triggered.connect(self.close)
        self.addAction(action)


    def __setup_menus_(self):
        self.__setup_file_menu_()
        self.__setup_edit_menu_()


    def __setup_file_menu_(self):
        menu = self.menuBar().addMenu("File")
        new = menu.addMenu("New")
        for action in self.__new_actions : new.addAction(action)
        menu.addAction(self.__open_action)
        menu.addAction(self.__save_action)
        menu.addAction(self.__save_as_action)
        menu.addAction(self.__close_action)
        menu.addSeparator()
        menu.addAction(self.__exit_action)


    def __setup_edit_menu_(self):
        self.menuBar().addMenu(self.__view.context_menu())


    @QtSlot(str)
    def __new_(self, lang_name):
        window = self
        if self.__model : window = Main_Window()
        window.__model.new(lang_name)
        window.__update_title_()
        window.setWindowModified(False)
        window.__update_actions_()
        window.show()


    @QtSlot()
    def __open_(self):
        self.__path,type_ = QFileDialog.getOpenFileName(self,"Open Project","","Socrates' Project File (*.spr)")
        if not self.__path :
            self.__path = None
            return
        window = self
        if self.__model : window = Main_Window()
        try:
            window.__model.load(self.__path)
        except:
            if window is self : self.__path = None
            else : window.deleteLater()
            raise
        window.__update_title_()
        window.setWindowModified(False)
        window.__update_actions_()
        window.show()


    @QtSlot()
    def __save_(self):
        if self.__path is None : self.__save_as_()
        else :
            self.__model.save(self.__path)
            self.setWindowModified(False)


    @QtSlot()
    def __save_as_(self):
        self.__path,type_ = QFileDialog.getSaveFileName(self,"Save Project","","Socrates' Project File (*.spr)")
        if not self.__path :
            self.__path = None
            return
        try:
            self.__model.save(self.__path)
        except:
            self.__path = None
            raise
        self.setWindowModified(False)
        self.__update_actions_()


    @QtSlot()
    def __close_(self):
        if self.__is_ok_to_close_() :
            self.__model.close()
            self.__update_title_()
            self.setWindowModified(False)
            self.__update_actions_()


    @QtSlot(str)
    def __name_changed_(self, name):
        self.__update_title_()


    @QtSlot()
    def __modified_(self):
        self.setWindowModified(True)


    #
    __instances = []
