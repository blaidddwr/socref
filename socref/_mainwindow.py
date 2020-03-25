"""
Contains the MainWindow class.
"""
import os
from PySide2 import QtCore as qtc
from PySide2 import QtGui as qtg
from PySide2 import QtWidgets as qtw
from . import abstract
from . import core
from . import gui
from . import settings








class MainWindow(qtw.QMainWindow):
    """
    This is the main window class. It is designed as a multiple window application. It is the main
    window of the core application.

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
        self.__model = core.ProjectModel(self)
        self.__view = gui.ProjectView(self)
        self.__blockViewDock = gui.BlockViewDock(self)
        self.__blockEditDock = gui.BlockEditDock(self)
        self.__progressBar = None
        self.__openAction = qtw.QAction("Open",self)
        self.__saveAction = qtw.QAction("Save",self)
        self.__saveAsAction = qtw.QAction("Save As",self)
        self.__closeAction = qtw.QAction("Close",self)
        self.__propertiesAction = qtw.QAction("Properties",self)
        self.__parseAction = qtw.QAction("Parse",self)
        self.__exitAction = qtw.QAction("Exit",self)
        self.__newActions = []
        self.__path = None
        self.__setupGui_()


    ####################
    # PUBLIC - Methods #
    ####################


    def open_(self, path):
        """
        Opens a project at the given path in this window. If this window already has a project then
        this does nothing.

        Parameters
        ----------
        path : string
               The path of the project file that this window opens.
        """
        if self.__model:
            return
        self.__model.load(path)
        self.__path = path
        self.__updateTitle_()
        self.setWindowModified(False)
        self.__updateActions_()


    ####################
    # PUBLIC - Signals #
    ####################


    #
    # Signals this window has started parsing its current project with the given abstract parser.
    #
    parseRequested = qtc.Signal(abstract.AbstractParser)


    #######################
    # PROTECTED - Methods #
    #######################


    def closeEvent(self, event):
        """
        Implements the PySide2.QtWidgets.QWidget interface.

        Parameters
        ----------
        event : object
                See qt docs.
        """
        if self.__isOkToClose_():
            settings = qtc.QSettings()
            settings.setValue(self.__GEOMETRY_KEY,self.saveGeometry())
            settings.setValue(self.__STATE_KEY,self.saveState())
            self.__instances.remove(self)
            self.deleteLater()
            event.accept()
        else:
            event.ignore()


    #####################
    # PRIVATE - Methods #
    #####################


    def __isOkToClose_(self):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if it is OK to close this window or false otherwise. It is OK to close this
               window if the user saves any unsaved changes to this window's project, chooses to
               discard unsaved changes, or there are no unsaved changes to worry about.
        """
        if not self.__model or not self.__model.isModified():
            return True
        answer = qtw.QMessageBox.question(
            self
            ,"Unsaved Project Changes"
            ,"The currently open project has unsaved changes. Closing the project will cause all"
             " unsaved changes to be lost!"
            ,qtw.QMessageBox.Save|qtw.QMessageBox.Cancel|qtw.QMessageBox.Discard
        )
        if answer == qtw.QMessageBox.Save:
            if self.__path is None:
                return self.__saveAs_()
            else:
                return self.__save_()
        elif answer == qtw.QMessageBox.Cancel:
            return False
        else:
            return True


    def __restore_(self):
        """
        Restores the geometry and state of this window.
        """
        settings = qtc.QSettings()
        geometry = settings.value(self.__GEOMETRY_KEY)
        state = settings.value(self.__STATE_KEY)
        if geometry:
            self.restoreGeometry(geometry)
        if state:
            self.restoreState(state)


    def __setupActions_(self):
        """
        Initializes all qt actions of this new window.
        """
        self.__setupNewActions_()
        self.__setupFileActions_()


    def __setupDocks_(self):
        """
        Initializes the edit/view block dock widgets of this new window.
        """
        self.__blockViewDock.setObjectName("block.view.dock")
        self.__blockEditDock.setObjectName("block.edit.dock")
        self.__blockViewDock.setWindowTitle("View")
        self.__blockEditDock.setWindowTitle("Edit")
        self.__blockViewDock.setView(self.__view)
        self.__blockEditDock.setView(self.__view)
        self.addDockWidget(qtc.Qt.RightDockWidgetArea,self.__blockViewDock)
        self.addDockWidget(qtc.Qt.RightDockWidgetArea,self.__blockEditDock)


    def __setupEditMenu_(self):
        """
        Adds this new window's project view's context menu as the window's edit menu.
        """
        self.menuBar().addMenu(self.__view.contextMenu())


    def __setupFileActions_(self):
        """
        Initializes the qt actions of this new window's file menu.
        """
        action = self.__openAction
        action.setIcon(qtg.QIcon.fromTheme("document-open"))
        action.setStatusTip("Open an existing project.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Open))
        action.triggered.connect(self.__open_)
        self.addAction(action)
        action = self.__saveAction
        action.setIcon(qtg.QIcon.fromTheme("document-save"))
        action.setStatusTip("Save the current project.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Save))
        action.triggered.connect(self.__save_)
        self.addAction(action)
        action = self.__saveAsAction
        action.setIcon(qtg.QIcon.fromTheme("document-save-as"))
        action.setStatusTip("Save the current project to a provided file path.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.SaveAs))
        action.triggered.connect(self.__saveAs_)
        self.addAction(action)
        action = self.__closeAction
        action.setStatusTip("Close the current project.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Close))
        action.triggered.connect(self.__close_)
        self.addAction(action)
        action = self.__propertiesAction
        action.setStatusTip("Open a dialog to edit the basic properties of the current project.")
        action.triggered.connect(self.__properties_)
        self.addAction(action)
        action = self.__parseAction
        action.setIcon(qtg.QIcon.fromTheme("view-refresh"))
        action.setStatusTip("Parse all source code files with the current project.")
        action.setShortcut(qtg.QKeySequence(qtc.Qt.CTRL + qtc.Qt.Key_P))
        action.triggered.connect(self.__parse_)
        self.addAction(action)
        action = self.__exitAction
        action.setIcon(qtg.QIcon.fromTheme("application-exit"))
        action.setStatusTip("Exit this window.")
        action.setShortcut(qtg.QKeySequence(qtg.QKeySequence.Quit))
        action.triggered.connect(self.close)
        self.addAction(action)


    def __setupFileMenu_(self):
        """
        Initializes the file menu for this new window.
        """
        menu = self.menuBar().addMenu("File")
        new = menu.addMenu("New")
        for action in self.__newActions:
            new.addAction(action)
        menu.addAction(self.__openAction)
        menu.addAction(self.__saveAction)
        menu.addAction(self.__saveAsAction)
        menu.addAction(self.__closeAction)
        menu.addSeparator()
        menu.addAction(self.__propertiesAction)
        menu.addAction(self.__parseAction)
        menu.addSeparator()
        menu.addAction(self.__exitAction)


    def __setupGui_(self):
        """
        Initializes the GUI of this new window.

        The status bar is also accessed that forces it to be visible before it is initially used in
        this new window.
        """
        self.__instances.append(self)
        self.setWindowIcon(qtg.QIcon(":/socref/application.svg"))
        self.__model.modified.connect(self.__modified_)
        self.__model.nameChanged.connect(self.__nameChanged_)
        self.__view.setModel(self.__model)
        self.setCentralWidget(self.__view)
        self.__setupDocks_()
        self.__setupActions_()
        self.__setupMenus_()
        self.__setupToolbars_()
        self.parseRequested.connect(core.parser.start)
        core.parser.started.connect(self.__parseStarted_)
        core.parser.progressed.connect(self.__parseProgressed_)
        core.parser.finished.connect(self.__parseFinished_)
        self.__restore_()
        self.statusBar()
        self.__updateTitle_()
        self.__updateActions_()


    def __setupHelpMenu_(self):
        """
        Initializes the help menu for this new window.
        """
        about = qtw.QAction("About",self)
        about.setStatusTip("Toggle the visibility of the block view dock.")
        about.triggered.connect(self.__about_)
        aboutqt = qtw.QAction("About Qt",self)
        aboutqt.setStatusTip("Toggle the visibility of the block edit dock.")
        aboutqt.triggered.connect(lambda : qtw.QMessageBox.aboutQt(self))
        menu = self.menuBar().addMenu("Help")
        menu.addAction(about)
        menu.addAction(aboutqt)


    def __setupMenus_(self):
        """
        Initializes all menus of this new window.
        """
        self.__setupFileMenu_()
        self.__setupEditMenu_()
        self.__setupViewMenu_()
        self.menuBar().addSeparator()
        self.__setupHelpMenu_()


    def __setupNewActions_(self):
        """
        Populates this window's list of new actions with all available languages.
        """
        for lang in core.blockFactory.langs():
            self.__newActions.append(qtw.QAction(lang,self))
            self.__newActions[-1].triggered.connect(
                lambda checked=False,name=lang : self.__new_(name)
            )
            self.addAction(self.__newActions[-1])


    def __setupToolbars_(self):
        """
        Initializes all toolbars of this new window.
        """
        toolbar = self.addToolBar("File")
        toolbar.setObjectName("file.toolbar")
        toolbar.addAction(self.__openAction)
        toolbar.addAction(self.__saveAction)
        toolbar.addAction(self.__saveAsAction)
        toolbar.addAction(self.__parseAction)
        toolbar = self.addToolBar("Edit")
        toolbar.setObjectName("edit.toolbar")
        toolbar.addAction(self.__view.undoAction())
        toolbar.addAction(self.__view.redoAction())
        toolbar.addAction(self.__view.removeAction())
        toolbar.addAction(self.__view.cutAction())
        toolbar.addAction(self.__view.copyAction())
        toolbar.addAction(self.__view.pasteAction())
        toolbar.addAction(self.__view.moveUpAction())
        toolbar.addAction(self.__view.moveDownAction())


    def __setupViewMenu_(self):
        """
        Initializes the view menu for this new window.
        """
        view = qtw.QAction("View Dock",self)
        view.setStatusTip("Toggle the visibility of the block view dock.")
        view.setCheckable(True)
        view.setChecked(self.__blockViewDock.isVisible())
        view.toggled.connect(self.__blockViewDock.setVisible)
        self.__blockViewDock.visibilityChanged.connect(view.setChecked)
        edit = qtw.QAction("Edit Dock",self)
        edit.setStatusTip("Toggle the visibility of the block edit dock.")
        edit.setCheckable(True)
        edit.setChecked(self.__blockEditDock.isVisible())
        edit.toggled.connect(self.__blockEditDock.setVisible)
        self.__blockEditDock.visibilityChanged.connect(edit.setChecked)
        menu = self.menuBar().addMenu("View")
        menu.addAction(view)
        menu.addAction(edit)


    def __updateActions_(self):
        """
        Updates this window's actions.
        """
        self.__saveAction.setDisabled(self.__path is None)
        self.__saveAsAction.setDisabled(not self.__model)
        self.__closeAction.setDisabled(not self.__model)
        self.__propertiesAction.setDisabled(not self.__model)
        self.__parseAction.setDisabled(self.__path is None)


    def __updateTitle_(self):
        """
        Updates this window's title.
        """
        if self.__model:
            self.setWindowTitle(
                "%s[*] (%s) - Socrates' Reference" % (self.__model.name(),self.__model.langName())
            )
        else:
            self.setWindowTitle("Socrates' Reference")


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot()
    def __about_(self):
        """
        Called to open a modal about dialog describing this application to the user.
        """
        ifile = qtc.QFile(":/socref/about.html")
        ifile.open(qtc.QIODevice.ReadOnly)
        text = ifile.readAll().data().decode(encoding="utf-8")
        ifile.close()
        qtw.QMessageBox.about(
            self
            ,"About Socrates' Reference"
            ,text.replace("%SOCREF_VER%",settings.VERSION)
        )


    @qtc.Slot()
    def __close_(self):
        """
        Called to close this window's current project. If this window has no project then this does
        nothing. If the current project has unsaved changes the user is queried about what to do.
        """
        if self.__model and self.__isOkToClose_():
            self.__model.close()
            self.__path = None
            self.__updateTitle_()
            self.setWindowModified(False)
            self.__updateActions_()


    @qtc.Slot()
    def __modified_(self):
        """
        Called to inform this window that its project has been modified.
        """
        self.setWindowModified(True)


    @qtc.Slot(str)
    def __nameChanged_(self, name):
        """
        Called to inform this window that its project's name has been changed. This updates the
        window's title.

        Parameters
        ----------
        name : string
               The new name of this window's model's project.
        """
        self.__updateTitle_()


    @qtc.Slot(str)
    def __new_(self, langName):
        """
        Called to create a new project of the given language for this window.

        Parameters
        ----------
        langName : string
                   The name of the language used to create a new project.
        """
        window = self
        if self.__model:
            window = MainWindow()
        window.__model.new(langName)
        window.__updateTitle_()
        window.setWindowModified(False)
        window.__updateActions_()
        window.show()


    @qtc.Slot()
    def __open_(self):
        """
        Called to open a project file for this window if it does not have a project or a new window
        otherwise.
        """
        path,type_ = qtw.QFileDialog.getOpenFileName(
            self
            ,"Open Project"
            ,""
            ,"Socrates' Project File (*.scp)"
        )
        if not path:
            return
        path = os.path.abspath(path)
        window = self
        if self.__model:
            window = MainWindow()
        try:
            window.open_(path)
        except:
            if window is not self:
                window.deleteLater()
            raise
        window.show()


    @qtc.Slot()
    def __parse_(self):
        """
        Called to parse the source code of this window's project. If this window does not have a
        project save path then this does nothing.
        """
        if self.__path is not None:
            parser = self.__model.parser()
            parser.setRootPath(
                os.path.abspath(
                    os.path.join(os.path.dirname(self.__path),self.__model.parsePath())
                )
            )
            self.parseRequested.emit(parser)


    @qtc.Slot()
    def __parseFinished_(self):
        """
        Called to inform this window that the singleton parser model has finished parsing.
        """
        if self.__progressBar is not None:
            self.__progressBar.deleteLater()
            self.__progressBar = None


    @qtc.Slot(int)
    def __parseProgressed_(self, percent):
        """
        Called to inform this window that the singleton parser model has made progress parsing.

        Parameters
        ----------
        percent : int
                  The percentage progress the singleton parser model has made parsing from 0 to 100.
        """
        if self.__progressBar is not None:
            self.__progressBar.setValue(percent)


    @qtc.Slot()
    def __parseStarted_(self):
        """
        Called to inform this window that the singleton parser model has started parsing.
        """
        if self.__progressBar is None:
            bar = self.__progressBar = qtw.QProgressBar()
            bar.setRange(0,100)
            bar.setValue(0)
            self.statusBar().addWidget(bar)
            bar.show()


    @qtc.Slot()
    def __properties_(self):
        """
        Called to have this window bring up a modal project dialog to edit the basic properties of
        its current project. If this window has no project then this does nothing.
        """
        gui.ProjectDialog(self.__model).exec_()


    @qtc.Slot()
    def __save_(self):
        """
        Called to save this window's project. If this window has no project or its save path is none
        then this does nothing.

        Returns
        -------
        ret0 : bool
               True if the project was saved successfully or false otherwise.
        """
        if not self.__model or self.__path is None:
            return False
        self.__model.save(self.__path)
        self.setWindowModified(False)
        return True


    @qtc.Slot()
    def __saveAs_(self):
        """
        Called to save this window's project to a new save file path selected by the user. If this
        window has no project then this does nothing.
        """
        if not self.__model:
            return False
        path,type_ = qtw.QFileDialog.getSaveFileName(
            self
            ,"Save Project"
            ,""
            ,"Socrates' Project File (*.scp)"
        )
        if not path:
            return False
        oldpath = self.__path
        self.__path = os.path.abspath(path)
        try:
            self.__model.save(self.__path)
        except:
            self.__path = oldpath
            raise
        self.setWindowModified(False)
        self.__updateActions_()
        return True


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
