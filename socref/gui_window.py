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
        #
        # Initialize this window's project model.
        #
        self.__model = model.Project(self)
        #
        # Initialize this window's project view.
        #
        self.__view = gui_view.Project(self)
        #
        # Initialize this window's view/edit block docks.
        #
        self.__block_view_dock = gui_view.Block_Dock(self)
        self.__block_edit_dock = gui_edit.Block_Dock(self)
        #
        # Initialize this window's actions.
        #
        self.__open_action = qtw.QAction("Open",self)
        self.__save_action = qtw.QAction("Save",self)
        self.__save_as_action = qtw.QAction("Save As",self)
        self.__parse_action = qtw.QAction("Parse",self)
        self.__close_action = qtw.QAction("Close",self)
        self.__exit_action = qtw.QAction("Exit",self)
        #
        # Initialize this window's empty list of new project actions.
        #
        self.__new_actions = []
        #
        # Initialize this window's project save path to none.
        #
        self.__path = None
        #
        # Append this new window instance to the global list so it is not deleted due to lack of
        # references.
        #
        self.__instances.append(self)
        #
        # Setup this window's project model and view.
        #
        self.__model.modified.connect(self.__modified_)
        self.__view.setModel(self.__model)
        self.setCentralWidget(self.__view)
        #
        # Setup this window's docks, actions, menus, and toolbars.
        #
        self.__setup_docks_()
        self.__setup_actions_()
        self.__setup_menus_()
        self.__setup_toolbars_()
        #
        # Connect this window's parse requested signal to the singleton parser model.
        #
        self.parse_requested.connect(model.Parser().start)
        #
        # Update this window's title and actions.
        #
        self.__update_title_()
        self.__update_actions_()


    ####################
    # PUBLIC - Signals #
    ####################


    #
    # Signals this window has started parsing its current project with the given abstract parser.
    #
    parse_requested = qtc.Signal(abstract.Parser)


    ####################
    # PUBLIC - Methods #
    ####################


    def closeEvent(self, event):
        """
        Implements the PySide2.QtWidgets.QWidget interface.

        event : See qt docs.
        """
        #
        # Check if it is OK to close this window.
        #
        if self.__is_ok_to_close_() :
            #
            # Remove this instance from the list of global main windows and delete it later.
            #
            self.__instances.remove(self)
            self.deleteLater()
            #
            # Accept the given event.
            #
            event.accept()
        #
        # Else it is not OK so ignore the given event.
        #
        else: event.ignore()


    #####################
    # PRIVATE - Methods #
    #####################


    def __update_actions_(self):
        """
        Updates this window's actions.
        """
        #
        # Disable this window's save and parse actions if its path is nothing.
        #
        self.__save_action.setDisabled(self.__path is None)
        self.__parse_action.setDisabled(self.__path is None)
        #
        # Disable this window's save as and close actions if it has no model.
        #
        self.__save_as_action.setDisabled(not self.__model)
        self.__close_action.setDisabled(not self.__model)


    def __is_ok_to_close_(self):
        """
        Getter method.

        return : True if it is OK to close this window or false otherwise. It is OK to close this
                 window if the user saves any unsaved changes to this window's project, chooses to
                 discard unsaved changes, or there are no unsaved changes to worry about.
        """
        #
        # If this window has no project or its project is not modified then return true.
        #
        if not self.__model or not self.__model.is_modified() : return True
        #
        # Initialize a message box informing the user there are unsaved changes.
        #
        box = qtw.QMessageBox()
        box.setWindowTitle("Unsaved Project Changes")
        box.setText("The currently open project has unsaved changes. Closing the project will cause"
                    " all unsaved changes to be lost!")
        box.setIcon(qtw.QMessageBox.Question)
        box.setStandardButtons(qtw.QMessageBox.Save
                               | qtw.QMessageBox.Cancel
                               | qtw.QMessageBox.Discard)
        #
        # Modally execute the message box, saving the user's response.
        #
        result = box.exec_()
        #
        # Check if the user response was to save the changes.
        #
        if result == qtw.QMessageBox.Save :
            #
            # If this is a new project that has never been saved call this window's save as slot,
            # else call its save slot. Either way return the return value of the called slot.
            #
            if self.__path is None : return self.__save_as_()
            else: return self.__save_()
        #
        # Else if the user response was to cancel closing then return false.
        #
        elif result == qtw.QMessageBox.Cancel : return False
        #
        # Else the user response was to discard so return true.
        #
        else: return True


    def __update_title_(self):
        """
        Updates this window's title.
        """
        #
        # It this window has a project then set its title to the project's name and application
        # name.
        #
        if self.__model :
            self.setWindowTitle("%s[*] (%s) - Socrates' Reference"
                                % (self.__model.name(),self.__model.lang_name()))
        #
        # Else this window has no project so set its title to the application name.
        #
        else:
            self.setWindowTitle("Socrates' Reference")


    def __setup_actions_(self):
        """
        Initializes all qt actions of this new window.
        """
        self.__setup_new_actions_()
        self.__setup_file_actions_()


    def __setup_new_actions_(self):
        """
        Populates this window's list of new actions with all available languages.
        """
        #
        # Iterate through all available languages.
        #
        for lang in abstract.Factory().langs() :
            #
            # Initialize and add a new action to this window's list for the language.
            #
            self.__new_actions.append(qtw.QAction(lang,self))
            self.__new_actions[-1].triggered.connect(lambda checked=False,name=lang : self.__new_(name))
            self.addAction(self.__new_actions[-1])


    def __setup_file_actions_(self):
        """
        Initializes the qt actions of this new window's file menu.
        """
        #
        # Initialize this window's open action.
        #
        action = self.__open_action
        action.setIcon(qtg.QIcon.fromTheme("document-open"))
        action.setStatusTip("Open an existing project.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Open))
        action.triggered.connect(self.__open_)
        self.addAction(action)
        #
        # Initialize this window's save action.
        #
        action = self.__save_action
        action.setIcon(qtg.QIcon.fromTheme("document-save"))
        action.setStatusTip("Save the current project.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Save))
        action.triggered.connect(self.__save_)
        self.addAction(action)
        #
        # Initialize this window's save as action.
        #
        action = self.__save_as_action
        action.setIcon(qtg.QIcon.fromTheme("document-save-as"))
        action.setStatusTip("Save the current project to a provided file path.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.SaveAs))
        action.triggered.connect(self.__save_as_)
        self.addAction(action)
        #
        # Initialize this window's parse action.
        #
        action = self.__parse_action
        action.setStatusTip("Parse all source code files with the current project.")
        action.setShortcut(qtg.QKeySequence(qtc.Qt.CTRL + qtc.Qt.Key_P))
        action.triggered.connect(self.__parse_)
        self.addAction(action)
        #
        # Initialize this window's close action.
        #
        action = self.__close_action
        action.setStatusTip("Close the current project.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Close))
        action.triggered.connect(self.__close_)
        self.addAction(action)
        #
        # Initialize this window's exit action.
        #
        action = self.__exit_action
        action.setIcon(qtg.QIcon.fromTheme("application-exit"))
        action.setStatusTip("Exit this window.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Quit))
        action.triggered.connect(self.close)
        self.addAction(action)


    def __setup_menus_(self):
        """
        Initializes all menus of this new window.
        """
        self.__setup_file_menu_()
        self.__setup_edit_menu_()


    def __setup_file_menu_(self):
        """
        Initializes the file menu for this new window.
        """
        #
        # Add the new file menu to this window's menu bar.
        #
        menu = self.menuBar().addMenu("File")
        #
        # Add the new sub menu. Iterate through all add actions and add the to the sub menu.
        #
        new = menu.addMenu("New")
        for action in self.__new_actions : new.addAction(action)
        #
        # Add the open, save, save as, and close actions to the file menu followed by a separator.
        #
        menu.addAction(self.__open_action)
        menu.addAction(self.__save_action)
        menu.addAction(self.__save_as_action)
        menu.addAction(self.__close_action)
        menu.addSeparator()
        #
        # Add the parse action to the file menu followed by a separator.
        #
        menu.addAction(self.__parse_action)
        menu.addSeparator()
        #
        # Add the exit action to the file menu.
        #
        menu.addAction(self.__exit_action)


    def __setup_edit_menu_(self):
        """
        Adds this new window's project view's context menu as the window's edit menu.
        """
        self.menuBar().addMenu(self.__view.context_menu())


    def __setup_docks_(self):
        """
        Initializes the edit/view block dock widgets of this new window.
        """
        #
        # Set both dock's views to this window's project view.
        #
        self.__block_view_dock.set_view(self.__view)
        self.__block_edit_dock.set_view(self.__view)
        #
        # Add both docks to this window.
        #
        self.addDockWidget(qtc.Qt.RightDockWidgetArea,self.__block_view_dock)
        self.addDockWidget(qtc.Qt.RightDockWidgetArea,self.__block_edit_dock)


    def __setup_toolbars_(self):
        """
        Initializes all toolbars of this new window.
        """
        #
        # Add the file toolbar to this window. Add the open, save and save as actions to the
        # toolbar.
        #
        toolbar = self.addToolBar("File")
        toolbar.addAction(self.__open_action)
        toolbar.addAction(self.__save_action)
        toolbar.addAction(self.__save_as_action)
        #
        # Add the edit toolbar to this window. Add this window's project view's undo, redo, remove,
        # cut, copy, paste, move up, and move down actions to the toolbar.
        #
        toolbar = self.addToolBar("Edit")
        toolbar.addAction(self.__view.undo_action())
        toolbar.addAction(self.__view.redo_action())
        toolbar.addAction(self.__view.remove_action())
        toolbar.addAction(self.__view.cut_action())
        toolbar.addAction(self.__view.copy_action())
        toolbar.addAction(self.__view.paste_action())
        toolbar.addAction(self.__view.move_up_action())
        toolbar.addAction(self.__view.move_down_action())


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


    @qtc.Slot(str)
    def __new_(self, lang_name):
        """
        Called to create a new project of the given language for this window.

        lang_name : The name of the language used to create a new project.
        """
        #
        # Initialize a name to this window or a new window if this one already has a project.
        #
        window = self
        if self.__model : window = Main()
        #
        # Create a new project of the given language.
        #
        window.__model.new(lang_name)
        #
        # Update the windows title and actions.
        #
        window.__update_title_()
        window.setWindowModified(False)
        window.__update_actions_()
        #
        # Make sure the window is shown.
        #
        window.show()


    @qtc.Slot()
    def __open_(self):
        """
        Called to open a project file for this window if it does not have a project or a new window
        otherwise.
        """
        #
        # Get the path to the project file, making sure the user choose a valid file path.
        #
        path,type_ = qtw.QFileDialog.getOpenFileName(self
                                                     ,"Open Project"
                                                     ,""
                                                     ,"Socrates' Project File (*.scp)")
        if not path : return
        #
        # Get the absolute path of the file.
        #
        path = os.path.abspath(path)
        #
        # Initialize a name to this window or a new window if this one already has a project.
        #
        window = self
        if self.__model : window = Main()
        try:
            #
            # Load the project file at the user provided path.
            #
            window.__model.load(path)
        except:
            #
            # If any exception happens while loading then delete the window if it is not this
            # window. Raise the exception again.
            #
            if window is not self : window.deleteLater()
            raise
        #
        # Set the window's project save path.
        #
        window.__path = path
        #
        # Update the window's title, state, and actions.
        #
        window.__update_title_()
        window.setWindowModified(False)
        window.__update_actions_()
        #
        # Make sure the window is shown.
        #
        window.show()


    @qtc.Slot()
    def __save_(self):
        """
        Called to save this window's project. If this window has no project or its save path is none
        then this does nothing.

        return : True if the project was saved successfully or false otherwise.
        """
        #
        # Make sure this window has a project and save file path.
        #
        if not self.__model or self.__path is None : return False
        #
        # Save this window's project, changing its modified status to false.
        #
        self.__model.save(self.__path)
        self.setWindowModified(False)
        #
        # Return success.
        #
        return True


    @qtc.Slot()
    def __save_as_(self):
        """
        Called to save this window's project to a new save file path selected by the user. If this
        window has no project then this does nothing.
        """
        #
        # Make sure this window has a project.
        #
        if not self.__model : return False
        #
        # Get the new save file path, making sure the user choose a valid path.
        #
        path,type_ = qtw.QFileDialog.getSaveFileName(self
                                                     ,"Save Project"
                                                     ,""
                                                     ,"Socrates' Project File (*.scp)")
        if not path : return False
        #
        # Set this window's save file path to the new one in its absolute form, saving the old path.
        #
        oldpath = self.__path
        self.__path = os.path.abspath(path)
        try:
            #
            # Save this window's model.
            #
            self.__model.save(self.__path)
        except:
            #
            # If any exception occurred while saving to set this window's save file path back to the
            # previous value and raise the exception again.
            #
            self.__path = oldpath
            raise
        #
        # Update this window's modified state and actions.
        #
        self.setWindowModified(False)
        self.__update_actions_()
        #
        # Return success.
        #
        return True


    @qtc.Slot()
    def __parse_(self):
        """
        Called to parse the source code of this window's project. If this window does not have a
        project save path then this does nothing.
        """
        #
        # Make sure this window has a project save path.
        #
        if self.__path is not None :
            #
            # Create a new parser from this window's model and set its root path to the directory
            # path of its project save path.
            #
            parser = self.__model.parser()
            parser.set_root_path(os.path.dirname(self.__path))
            #
            # Signal the parsing is requested with the new parser.
            #
            self.parse_requested.emit(parser)


    @qtc.Slot()
    def __close_(self):
        """
        Called to close this window's current project. If this window has no project then this does
        nothing. If the current project has unsaved changes the user is queried about what to do.
        """
        #
        # Make sure this window has a project and it is OK to close.
        #
        if self.__model and self.__is_ok_to_close_() :
            #
            # Close this window's model's project.
            #
            self.__model.close()
            #
            # Set this window's save path to nothing.
            #
            self.__path = None
            #
            # Update this window's title, state, and actions.
            #
            self.__update_title_()
            self.setWindowModified(False)
            self.__update_actions_()


    ############################
    # PRIVATE - Static Objects #
    ############################


    #
    # List of all active main window instances. Used to make sure they are not prematurely deleted.
    #
    __instances = []
