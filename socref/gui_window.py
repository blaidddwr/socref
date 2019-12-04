"""
Contains all GUI windows for the core application.
"""
import os
from PySide2 import QtCore as qtc
from PySide2 import QtGui as qtg
from PySide2 import QtWidgets as qtw
from . import util
from . import abstract
from . import model
from . import gui_view
from . import gui_edit
from . import gui_dialog








class Main(qtw.QMainWindow):
    """
    This is the main window class. It inherits the qt main window class. It is designed as a
    multiple window application. It is the main window of the core application.

    It is designed as a multiple window application. Any number of these windows can be open, the
    application itself will close when the last main window closes. Each main window can edit its
    own project independent of all other main windows. A reference does not need to be saved for
    main windows because it keeps its own list of global instances within its class object.

    It is the main window of the core application. It provides a view, docks, menus, and a toolbar
    that allows a user to work on any project in the application. The project view actually displays
    the tree of blocks of an open project. The view dock provides a detailed view of the current
    block in the project view. The edit dock provides an edit form for modifying the current block's
    properties. The menus and toolbars provide shortcuts for actions that can be done in the project
    view. They also provide basic actions that a main window handles itself such as opening or
    saving projects.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new main window.
        """
        qtw.QMainWindow.__init__(self)
        self.__model = model.Project(self)
        self.__view = gui_view.Project(self)
        self.__block_view_dock = gui_view.Block_Dock(self)
        self.__block_edit_dock = gui_edit.Block_Dock(self)
        self.__progress_bar = None
        self.__open_action = qtw.QAction("Open",self)
        self.__save_action = qtw.QAction("Save",self)
        self.__save_as_action = qtw.QAction("Save As",self)
        self.__close_action = qtw.QAction("Close",self)
        self.__properties_action = qtw.QAction("Properties",self)
        self.__parse_action = qtw.QAction("Parse",self)
        self.__exit_action = qtw.QAction("Exit",self)
        self.__new_actions = []
        self.__path = None
        self.__setup_gui_()


    ####################
    # PUBLIC - Signals #
    ####################


    #
    # Signals this window has started parsing its current project with the given abstract parser.
    #
    parse_requested = qtc.Signal(abstract.Parser)


    #######################
    # PROTECTED - Methods #
    #######################


    def closeEvent(self, event):
        """
        Implements the PySide2.QtWidgets.QWidget interface.

        event : See qt docs.
        """
        if self.__is_ok_to_close_() :
            settings = qtc.QSettings()
            settings.setValue(self.__GEOMETRY_KEY,self.saveGeometry())
            settings.setValue(self.__STATE_KEY,self.saveState())
            self.__instances.remove(self)
            self.deleteLater()
            event.accept()
        else: event.ignore()


    #####################
    # PRIVATE - Methods #
    #####################


    def __update_actions_(self):
        """
        Updates this window's actions.
        """
        self.__save_action.setDisabled(self.__path is None)
        self.__save_as_action.setDisabled(not self.__model)
        self.__close_action.setDisabled(not self.__model)
        self.__properties_action.setDisabled(not self.__model)
        self.__parse_action.setDisabled(self.__path is None)


    def __is_ok_to_close_(self):
        """
        Getter method.

        return : True if it is OK to close this window or false otherwise. It is OK to close this
                 window if the user saves any unsaved changes to this window's project, chooses to
                 discard unsaved changes, or there are no unsaved changes to worry about.
        """
        if not self.__model or not self.__model.is_modified() : return True
        answer = qtw.QMessageBox.question(self
                                          ,"Unsaved Project Changes"
                                          ,"The currently open project has unsaved changes. Closing"
                                           " the project will cause all unsaved changes to be lost!"
                                          ,qtw.QMessageBox.Save
                                           | qtw.QMessageBox.Cancel
                                           | qtw.QMessageBox.Discard)
        if answer == qtw.QMessageBox.Save :
            if self.__path is None : return self.__save_as_()
            else: return self.__save_()
        elif answer == qtw.QMessageBox.Cancel : return False
        else: return True


    def __update_title_(self):
        """
        Updates this window's title.
        """
        if self.__model :
            self.setWindowTitle("%s[*] (%s) - Socrates' Reference"
                                % (self.__model.name(),self.__model.lang_name()))
        else:
            self.setWindowTitle("Socrates' Reference")


    def __setup_gui_(self):
        """
        Initializes the GUI of this new window.
        """
        self.__instances.append(self)
        self.setWindowIcon(qtg.QIcon(":/socref/application.svg"))
        self.__model.modified.connect(self.__modified_)
        self.__model.name_changed.connect(self.__update_title_)
        self.__view.setModel(self.__model)
        self.setCentralWidget(self.__view)
        self.__setup_docks_()
        self.__setup_actions_()
        self.__setup_menus_()
        self.__setup_toolbars_()
        self.parse_requested.connect(model.Parser().start)
        model.Parser().started.connect(self.__parse_started_)
        model.Parser().progressed.connect(self.__parse_progressed_)
        model.Parser().finished.connect(self.__parse_finished_)
        self.__restore_()
        #
        # The status bar is not visible until this method is called so call it here to make it
        # visible from the start.
        #
        self.statusBar()
        self.__update_title_()
        self.__update_actions_()


    def __setup_actions_(self):
        """
        Initializes all qt actions of this new window.
        """
        self.__setup_new_actions_()
        self.__setup_file_actions_()


    def __setup_menus_(self):
        """
        Initializes all menus of this new window.
        """
        self.__setup_file_menu_()
        self.__setup_edit_menu_()


    def __setup_toolbars_(self):
        """
        Initializes all toolbars of this new window.
        """
        #
        # File toolbar.
        #
        toolbar = self.addToolBar("File")
        toolbar.setObjectName("file.toolbar")
        toolbar.addAction(self.__open_action)
        toolbar.addAction(self.__save_action)
        toolbar.addAction(self.__save_as_action)
        toolbar.addAction(self.__parse_action)
        #
        # Edit toolbar.
        #
        toolbar = self.addToolBar("Edit")
        toolbar.setObjectName("edit.toolbar")
        toolbar.addAction(self.__view.undo_action())
        toolbar.addAction(self.__view.redo_action())
        toolbar.addAction(self.__view.remove_action())
        toolbar.addAction(self.__view.cut_action())
        toolbar.addAction(self.__view.copy_action())
        toolbar.addAction(self.__view.paste_action())
        toolbar.addAction(self.__view.move_up_action())
        toolbar.addAction(self.__view.move_down_action())


    def __setup_docks_(self):
        """
        Initializes the edit/view block dock widgets of this new window.
        """
        self.__block_view_dock.setObjectName("block.view.dock")
        self.__block_edit_dock.setObjectName("block.edit.dock")
        self.__block_view_dock.setWindowTitle("View")
        self.__block_edit_dock.setWindowTitle("Edit")
        self.__block_view_dock.set_view(self.__view)
        self.__block_edit_dock.set_view(self.__view)
        self.addDockWidget(qtc.Qt.RightDockWidgetArea,self.__block_view_dock)
        self.addDockWidget(qtc.Qt.RightDockWidgetArea,self.__block_edit_dock)


    def __setup_new_actions_(self):
        """
        Populates this window's list of new actions with all available languages.
        """
        for lang in abstract.Factory().langs() :
            self.__new_actions.append(qtw.QAction(lang,self))
            self.__new_actions[-1].triggered.connect(lambda checked=False,name=lang : self.__new_(name))
            self.addAction(self.__new_actions[-1])


    def __setup_file_actions_(self):
        """
        Initializes the qt actions of this new window's file menu.
        """
        #
        # Open action.
        #
        action = self.__open_action
        action.setIcon(qtg.QIcon.fromTheme("document-open"))
        action.setStatusTip("Open an existing project.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Open))
        action.triggered.connect(self.__open_)
        self.addAction(action)
        #
        # Save action.
        #
        action = self.__save_action
        action.setIcon(qtg.QIcon.fromTheme("document-save"))
        action.setStatusTip("Save the current project.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Save))
        action.triggered.connect(self.__save_)
        self.addAction(action)
        #
        # Save as action.
        #
        action = self.__save_as_action
        action.setIcon(qtg.QIcon.fromTheme("document-save-as"))
        action.setStatusTip("Save the current project to a provided file path.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.SaveAs))
        action.triggered.connect(self.__save_as_)
        self.addAction(action)
        #
        # Close action.
        #
        action = self.__close_action
        action.setStatusTip("Close the current project.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Close))
        action.triggered.connect(self.__close_)
        self.addAction(action)
        #
        # Properties action.
        #
        action = self.__properties_action
        action.setStatusTip("Open a dialog to edit the basic properties of the current project.")
        action.triggered.connect(self.__properties_)
        self.addAction(action)
        #
        # Parse action.
        #
        action = self.__parse_action
        action.setIcon(qtg.QIcon.fromTheme("view-refresh"))
        action.setStatusTip("Parse all source code files with the current project.")
        action.setShortcut(qtg.QKeySequence(qtc.Qt.CTRL + qtc.Qt.Key_P))
        action.triggered.connect(self.__parse_)
        self.addAction(action)
        #
        # Exit action.
        #
        action = self.__exit_action
        action.setIcon(qtg.QIcon.fromTheme("application-exit"))
        action.setStatusTip("Exit this window.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Quit))
        action.triggered.connect(self.close)
        self.addAction(action)


    def __setup_file_menu_(self):
        """
        Initializes the file menu for this new window.
        """
        menu = self.menuBar().addMenu("File")
        new = menu.addMenu("New")
        for action in self.__new_actions : new.addAction(action)
        menu.addAction(self.__open_action)
        menu.addAction(self.__save_action)
        menu.addAction(self.__save_as_action)
        menu.addAction(self.__close_action)
        menu.addSeparator()
        menu.addAction(self.__properties_action)
        menu.addAction(self.__parse_action)
        menu.addSeparator()
        menu.addAction(self.__exit_action)


    def __setup_edit_menu_(self):
        """
        Adds this new window's project view's context menu as the window's edit menu.
        """
        self.menuBar().addMenu(self.__view.context_menu())


    def __restore_(self):
        """
        Restores the geometry and state of this window.
        """
        settings = qtc.QSettings()
        geometry = settings.value(self.__GEOMETRY_KEY)
        state = settings.value(self.__STATE_KEY)
        if geometry : self.restoreGeometry(geometry)
        if state : self.restoreState(state)


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot()
    def __modified_(self):
        """
        Called to inform this window that its project has been modified.
        """
        self.setWindowModified(True)


    @qtc.Slot(str)
    def __name_changed_(self, name):
        """
        Called to inform this window that its project's name has been changed. This updates the
        window's title.

        name : Detailed description.
        """
        self.__update_title_()


    @qtc.Slot()
    def __parse_started_(self):
        """
        Called to inform this window that the singleton parser model has started parsing.
        """
        if self.__progress_bar is None :
            bar = self.__progress_bar = qtw.QProgressBar()
            bar.setRange(0,100)
            bar.setValue(0)
            self.statusBar().addWidget(bar)
            bar.show()


    @qtc.Slot(int)
    def __parse_progressed_(self, percent):
        """
        Called to inform this window that the singleton parser model has made progress parsing.

        percent : The percentage progress the singleton parser model has made parsing from 0 to 100.
        """
        if self.__progress_bar is not None : self.__progress_bar.setValue(percent)


    @qtc.Slot()
    def __parse_finished_(self):
        """
        Called to inform this window that the singleton parser model has finished parsing.
        """
        if self.__progress_bar is not None :
            self.__progress_bar.deleteLater()
            self.__progress_bar = None


    @qtc.Slot(str)
    def __new_(self, lang_name):
        """
        Called to create a new project of the given language for this window.

        lang_name : The name of the language used to create a new project.
        """
        window = self
        if self.__model : window = Main()
        window.__model.new(lang_name)
        window.__update_title_()
        window.setWindowModified(False)
        window.__update_actions_()
        window.show()


    @qtc.Slot()
    def __open_(self):
        """
        Called to open a project file for this window if it does not have a project or a new window
        otherwise.
        """
        path,type_ = qtw.QFileDialog.getOpenFileName(self
                                                     ,"Open Project"
                                                     ,""
                                                     ,"Socrates' Project File (*.scp)")
        if not path : return
        path = os.path.abspath(path)
        window = self
        if self.__model : window = Main()
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
        """
        Called to save this window's project. If this window has no project or its save path is none
        then this does nothing.

        return : True if the project was saved successfully or false otherwise.
        """
        if not self.__model or self.__path is None : return False
        self.__model.save(self.__path)
        self.setWindowModified(False)
        return True


    @qtc.Slot()
    def __save_as_(self):
        """
        Called to save this window's project to a new save file path selected by the user. If this
        window has no project then this does nothing.
        """
        if not self.__model : return False
        path,type_ = qtw.QFileDialog.getSaveFileName(self
                                                     ,"Save Project"
                                                     ,""
                                                     ,"Socrates' Project File (*.scp)")
        if not path : return False
        oldpath = self.__path
        self.__path = os.path.abspath(path)
        try:
            self.__model.save(self.__path)
        except:
            self.__path = oldpath
            raise
        self.setWindowModified(False)
        self.__update_actions_()
        return True


    @qtc.Slot()
    def __close_(self):
        """
        Called to close this window's current project. If this window has no project then this does
        nothing. If the current project has unsaved changes the user is queried about what to do.
        """
        if self.__model and self.__is_ok_to_close_() :
            self.__model.close()
            self.__path = None
            self.__update_title_()
            self.setWindowModified(False)
            self.__update_actions_()


    @qtc.Slot()
    def __parse_(self):
        """
        Called to parse the source code of this window's project. If this window does not have a
        project save path then this does nothing.
        """
        if self.__path is not None :
            parser = self.__model.parser()
            parser.set_root_path(os.path.abspath(os.path.join(os.path.dirname(self.__path),self.__model.parse_path())))
            self.parse_requested.emit(parser)


    def __properties_(self):
        """
        Called to have this window bring up a modal project dialog to edit the basic properties of
        its current project. If this window has no project then this does nothing.
        """
        gui_dialog.Project(self.__model).exec_()


    ############################
    # PRIVATE - Static Objects #
    ############################


    #
    # List of all active main window instances. Used to make sure they are not prematurely deleted.
    #
    __instances = []


    #######################
    # PRIVATE - Constants #
    #######################


    #
    # The key used to save this window's geometry using qt settings to make it persistent.
    #
    __GEOMETRY_KEY = "gui.window.main.geometry"
    #
    # The key used to save this window's state using qt settings to make it persistent.
    #
    __STATE_KEY = "gui.window.main.state"
