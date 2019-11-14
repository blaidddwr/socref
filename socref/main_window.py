"""
todo
"""
import os
from PySide2 import QtCore as qtc
from PySide2 import QtGui as qtg
from PySide2 import QtWidgets as qtw
from . import project_model as pm
from . import project_view as pv
from . import block_view_dock as bvd
from . import block_edit_dock as bed
from . import block_factory as bf
from . import parser_model as pam
from . import abstract_parser as ap






class Main_Window(qtw.QMainWindow):


    def __init__(self):
        qtw.QMainWindow.__init__(self)
        self.__model = pm.Project_Model(self)
        self.__view = pv.Project_View(self)
        self.__block_view_dock = bvd.Block_View_Dock(self)
        self.__block_edit_dock = bed.Block_Edit_Dock(self)
        self.__open_action = qtw.QAction("Open",self)
        self.__save_action = qtw.QAction("Save",self)
        self.__save_as_action = qtw.QAction("Save As",self)
        self.__parse_action = qtw.QAction("Parse",self)
        self.__close_action = qtw.QAction("Close",self)
        self.__exit_action = qtw.QAction("Exit",self)
        self.__new_actions = []
        self.__path = None
        self.__instances.append(self)
        self.__model.modified.connect(self.__modified_)
        self.__view.setModel(self.__model)
        self.__setup_docks_()
        self.__setup_actions_()
        self.__setup_menus_()
        self.__setup_toolbars_()
        self.__update_title_()
        self.__update_actions_()
        self.parse_requested.connect(pam.Parser_Model().start)


    parse_requested = qtc.Signal(ap.Abstract_Parser)


    def closeEvent(self, event):
        if self.__is_ok_to_close_() : event.accept()
        else: event.ignore()


    def __update_actions_(self):
        self.__save_action.setDisabled(self.__path is None)
        self.__save_as_action.setDisabled(not self.__model)
        self.__close_action.setDisabled(not self.__model)


    def __is_ok_to_close_(self):
        if not self.__model or not self.__model.is_modified() : return True
        box = qtw.QMessageBox()
        box.setWindowTitle("Unsaved Project Changes")
        box.setText("The currently open project has unsaved changes. Closing the project will cause all unsaved changes to be lost!")
        box.setIcon(qtw.QMessageBox.Question)
        box.setStandardButtons(qtw.QMessageBox.Save | qtw.QMessageBox.Cancel | qtw.QMessageBox.Discard)
        result = box.exec_()
        if result == qtw.QMessageBox.Save :
            if self.__path is None : return self.__save_as_()
            else: return self.__save_()
        elif result == qtw.QMessageBox.Cancel : return False
        else: return True


    def __update_title_(self):
        if self.__model :
            self.setWindowTitle("%s[*] (%s) - Socrates' Reference"
                                % (self.__model.name(),self.__model.lang_name()))
        else:
            self.setWindowTitle("Socrates' Reference")


    def __setup_actions_(self):
        self.__setup_new_actions_()
        self.__setup_file_actions_()


    def __setup_new_actions_(self):
        for lang in bf.Block_Factory().langs() :
            self.__new_actions.append(qtw.QAction(lang,self))
            self.__new_actions[-1].triggered.connect(lambda checked=False,name=lang : self.__new_(name))
            self.addAction(self.__new_actions[-1])


    def __setup_file_actions_(self):
        #
        action = self.__open_action
        action.setIcon(qtg.QIcon.fromTheme("document-open"))
        action.setStatusTip("Open an existing project.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Open))
        action.triggered.connect(self.__open_)
        self.addAction(action)
        #
        action = self.__save_action
        action.setIcon(qtg.QIcon.fromTheme("document-save"))
        action.setStatusTip("Save the current project.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Save))
        action.triggered.connect(self.__save_)
        self.addAction(action)
        #
        action = self.__save_as_action
        action.setIcon(qtg.QIcon.fromTheme("document-save-as"))
        action.setStatusTip("Save the current project to a provided file path.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.SaveAs))
        action.triggered.connect(self.__save_as_)
        self.addAction(action)
        #
        action = self.__parse_action
        action.setStatusTip("Parse all source code files with the current project.")
        action.setShortcut(qtg.QKeySequence(qtc.Qt.CTRL + qtc.Qt.Key_P))
        action.triggered.connect(self.__parse_)
        self.addAction(action)
        #
        action = self.__close_action
        action.setStatusTip("Close the current project.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Close))
        action.triggered.connect(self.__close_)
        self.addAction(action)
        #
        action = self.__exit_action
        action.setIcon(qtg.QIcon.fromTheme("application-exit"))
        action.setStatusTip("Exit this window.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Quit))
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
        menu.addAction(self.__parse_action)
        menu.addSeparator()
        menu.addAction(self.__exit_action)


    def __setup_edit_menu_(self):
        self.menuBar().addMenu(self.__view.context_menu())


    def __setup_docks_(self):
        self.__block_view_dock.set_view(self.__view)
        self.__block_edit_dock.set_view(self.__view)
        self.setCentralWidget(self.__view)
        self.addDockWidget(qtc.Qt.RightDockWidgetArea,self.__block_view_dock)
        self.addDockWidget(qtc.Qt.RightDockWidgetArea,self.__block_edit_dock)


    def __setup_toolbars_(self):
        toolbar = self.addToolBar("File")
        toolbar.addAction(self.__open_action)
        toolbar.addAction(self.__save_action)
        toolbar.addAction(self.__save_as_action)
        toolbar = self.addToolBar("Edit")
        toolbar.addAction(self.__view.undo_action())
        toolbar.addAction(self.__view.redo_action())
        toolbar.addAction(self.__view.remove_action())
        toolbar.addAction(self.__view.cut_action())
        toolbar.addAction(self.__view.copy_action())
        toolbar.addAction(self.__view.paste_action())
        toolbar.addAction(self.__view.move_up_action())
        toolbar.addAction(self.__view.move_down_action())


    @qtc.Slot(str)
    def __new_(self, lang_name):
        window = self
        if self.__model : window = Main_Window()
        window.__model.new(lang_name)
        window.__update_title_()
        window.setWindowModified(False)
        window.__update_actions_()
        window.show()


    @qtc.Slot()
    def __open_(self):
        path,type_ = qtw.QFileDialog.getOpenFileName(self,"Open Project","","Socrates' Project File (*.scp)")
        if not path : return
        path = os.path.abspath(path)
        window = self
        if self.__model : window = Main_Window()
        try:
            window.__model.load(path)
        except:
            if window is not self : window.deleteLater()
            raise
        window.__path = path
        window.__update_title_()
        window.setWindowModified(False)
        window.__update_actions_()
        window.show()


    @qtc.Slot()
    def __save_(self):
        if self.__path is None : self.__save_as_()
        else:
            self.__model.save(self.__path)
            self.setWindowModified(False)


    @qtc.Slot()
    def __save_as_(self):
        self.__path,type_ = qtw.QFileDialog.getSaveFileName(self,"Save Project","","Socrates' Project File (*.scp)")
        if not self.__path :
            self.__path = None
            return
        self.__path = os.path.abspath(self.__path)
        try:
            self.__model.save(self.__path)
        except:
            self.__path = None
            raise
        self.setWindowModified(False)
        self.__update_actions_()


    @qtc.Slot()
    def __parse_(self):
        if self.__path is not None :
            parser = self.__model.parser()
            parser.set_root_path(os.path.dirname(self.__path))
            self.parse_requested.emit(parser)


    @qtc.Slot()
    def __close_(self):
        if self.__is_ok_to_close_() :
            self.__model.close()
            self.__update_title_()
            self.setWindowModified(False)
            self.__update_actions_()


    @qtc.Slot(str)
    def __name_changed_(self, name):
        self.__update_title_()


    @qtc.Slot()
    def __modified_(self):
        self.setWindowModified(True)


    #
    __instances = []
