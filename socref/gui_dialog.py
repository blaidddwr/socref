"""
Contains all GUI dialog windows used by the core application.
"""
import os
from PySide2 import QtCore as qtc
from PySide2 import QtGui as qtg
from PySide2 import QtWidgets as qtw
from . import settings
from . import gui_util
from . import gui_edit








class TextDialog(qtw.QDialog):
    """
    This is the text dialog class. It is a plain text editor with optional spell checking and
    correction. Any misspelled words are highlighted and a button is provided that begins spell
    checking and correction. All spell checking features can be enabled or disabled at
    initialization of the dialog.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, text="", parent=None, speller=False):
        """
        Initializes a new text dialog with the given optional text and parent. Spelling features can
        also be enabled or disabled.

        text : The optional text of this new text dialog.

        parent : The optional qt object parent of this new text dialog.

        speller : True to enable misspelled word highlighting and spell checking or false to disable
                  it all.
        """
        qtw.QDialog.__init__(self,parent)
        self.__speller = speller
        self.__text_edit = gui_edit.PlainTextEdit(text,self,speller=speller,popup=False)
        self.__speller_box = gui_util.SpellChecker("Spell Check",settings.DICTIONARY)
        self.__spell_button = qtw.QPushButton("Spell Check",self)
        self.__setup_gui_()


    ####################
    # PUBLIC - Methods #
    ####################


    def text(self):
        """
        Getter method.

        return : The current text of this text dialog.
        """
        return self.__text_edit.toPlainText()


    #######################
    # PROTECTED - Methods #
    #######################


    def closeEvent(self, event):
        """
        Implements the PySide2.QtWidgets.QWidget interface.

        event : See qt docs.
        """
        settings = qtc.QSettings()
        settings.setValue(self.__GEOMETRY_KEY,self.saveGeometry())
        event.accept()


    #####################
    # PRIVATE - Methods #
    #####################


    def __setup_gui_(self):
        """
        Initializes the GUI of this new text dialog.
        """
        layout = qtw.QVBoxLayout()
        layout.addWidget(self.__text_edit)
        layout.addWidget(self.__setup_spell_checker_())
        layout.addLayout(self.__setup_buttons_())
        self.setLayout(layout)
        self.__restore_()


    def __setup_spell_checker_(self):
        """
        Initializes the spell checker box of this new text dialog.

        return : The initialized spell checker box widget.
        """
        self.__speller_box.hide()
        self.__speller_box.cursor_changed.connect(
            lambda cursor : self.__text_edit.setTextCursor(cursor)
        )
        self.__speller_box.finished.connect(self.__spell_check_finished_)
        return self.__speller_box


    def __setup_buttons_(self):
        """
        Initializes the buttons of this new text dialog.

        return : A box layout of initialized buttons.
        """
        set_ = qtw.QPushButton("Set")
        set_.clicked.connect(lambda : self.done(qtw.QDialog.Accepted))
        cancel = qtw.QPushButton("Cancel")
        cancel.clicked.connect(lambda : self.done(qtw.QDialog.Rejected))
        self.__spell_button.clicked.connect(self.__spell_check_)
        self.__spell_button.setDisabled(not self.__speller)
        ret = qtw.QHBoxLayout()
        ret.addWidget(set_)
        ret.addWidget(cancel)
        ret.addStretch()
        ret.addWidget(self.__spell_button)
        return ret


    def __restore_(self):
        """
        Restores the geometry of this dialog.
        """
        settings = qtc.QSettings()
        geometry = settings.value(self.__GEOMETRY_KEY)
        if geometry:
            self.restoreGeometry(geometry)


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot()
    def __spell_check_(self):
        """
        Called to begin spell checking this text dialog's document. This shows its spell checker box
        and tells it to start checking.
        """
        self.__speller_box.show()
        self.__speller_box.start(self.__text_edit.textCursor())


    @qtc.Slot()
    def __spell_check_finished_(self):
        """
        Called to inform this text dialog that spell checking has finished. This hides its spell
        checker box and clears any selection from its document's cursor.
        """
        self.__speller_box.hide()
        cursor = self.__text_edit.textCursor()
        cursor.clearSelection()
        self.__text_edit.setTextCursor(cursor)


    #######################
    # PRIVATE - Constants #
    #######################


    #
    # The key used to save this dialog's geometry using qt settings to make it persistent.
    #
    __GEOMETRY_KEY = "gui.dialog.text.geometry"








class ProjectDialog(qtw.QDialog):
    """
    This is the project dialog class. It is a basic form dialog that allows the user to edit the
    project name and parse path of the model it is given on initialization. It is a persistent
    dialog that remembers its geometry.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, model, parent=None):
        """
        Initializes a new project dialog with the given model and optional parent.

        model : The project model that this dialog edits with its form.

        parent : The optional qt object parent of this dialog.
        """
        qtw.QDialog.__init__(self,parent)
        self.__name_edit = qtw.QLineEdit(self)
        self.__parse_path_edit = qtw.QLineEdit(self)
        self.__model = model
        model.name_changed.connect(self.__name_changed_)
        model.parse_path_changed.connect(self.__parse_path_changed_)
        self.__setup_gui_()


    #######################
    # PROTECTED - Methods #
    #######################


    def closeEvent(self, event):
        """
        Implements the PySide2.QtWidgets.QWidget interface.

        event : See qt docs.
        """
        settings = qtc.QSettings()
        settings.setValue(self.__GEOMETRY_KEY,self.saveGeometry())
        event.accept()


    #####################
    # PRIVATE - Methods #
    #####################


    def __setup_gui_(self):
        """
        Initializes the GUI of this new dialog.
        """
        layout = qtw.QVBoxLayout()
        layout.addLayout(self.__setup_form_())
        layout.addLayout(self.__setup_buttons_())
        self.setLayout(layout)
        self.setWindowTitle("Project Properties")
        self.__restore_()


    def __setup_form_(self):
        """
        Initializes the form of this new dialog.
        """
        self.__name_edit.setText(self.__model.name())
        self.__parse_path_edit.setText(self.__model.parse_path())
        ret = qtw.QFormLayout()
        ret.addRow("Project Name:",self.__name_edit)
        ret.addRow("Parsing Path:",self.__parse_path_edit)
        return ret


    def __setup_buttons_(self):
        """
        Initializes the buttons of this new dialog.
        """
        ok = qtw.QPushButton("Ok")
        ok.clicked.connect(self.__ok_)
        apply_ = qtw.QPushButton("Apply")
        apply_.clicked.connect(self.__apply_)
        cancel = qtw.QPushButton("Cancel")
        cancel.clicked.connect(self.__cancel_)
        ret = qtw.QHBoxLayout()
        ret.addWidget(ok)
        ret.addWidget(apply_)
        ret.addStretch()
        ret.addWidget(cancel)
        return ret


    def __restore_(self):
        """
        Restores the geometry of this dialog.
        """
        settings = qtc.QSettings()
        geometry = settings.value(self.__GEOMETRY_KEY)
        if geometry:
            self.restoreGeometry(geometry)


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot(str)
    def __name_changed_(self, name):
        """
        Called to inform this dialog its project model's name has changed.

        name : The new name of this dialog's project model.
        """
        self.__name_edit.setText(name)


    @qtc.Slot(str)
    def __parse_path_changed_(self, path):
        """
        Called to inform this dialog its project model's parse path has changed.

        path : The new parse path of this dialog's project model.
        """
        self.__parse_path_edit.setText(path)


    @qtc.Slot()
    def __ok_(self):
        """
        Called to apply this dialog's edit widget values to its project model and then close itself.
        """
        self.__apply_()
        self.done(qtw.QDialog.Accepted)
        self.close()


    @qtc.Slot()
    def __apply_(self):
        """
        Called to apply this dialog's edit widget values to its project model.
        """
        self.__model.set_name(self.__name_edit.text())
        self.__model.set_parse_path(self.__parse_path_edit.text())


    @qtc.Slot()
    def __cancel_(self):
        """
        Called to cancel this dialog, closing it and not applying any changes made in its edit
        widgets.
        """
        self.done(qtw.QDialog.Rejected)
        self.close()


    #######################
    # PRIVATE - Constants #
    #######################


    #
    # The key used to save this dialog's geometry using qt settings to make it persistent.
    #
    __GEOMETRY_KEY = "gui.dialog.project.geometry"








class CodeDialog(qtw.QDialog):
    """
    This is the code dialog class. It displays any unknown code fragments given to it. The fragments
    must be organized into a flat dictionary where the key should provide a meaningful identity
    about the code fragment to the user. The user is provided a list and read only text view to
    navigate the fragments, along with a copy to clipboard button.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, code, parent=None):
        """
        Initializes a new code dialog with the given code dictionary and optional parent.

        code : The code dictionary that this dialog displays to the user.

        parent : The optional qt object parent of this dialog.
        """
        qtw.QDialog.__init__(self,parent)
        self.__code = code
        self.__splitter = qtw.QSplitter(self)
        self.__view = qtw.QPlainTextEdit(self,readOnly=True)
        self.__setup_gui_()


    #######################
    # PROTECTED - Methods #
    #######################


    def closeEvent(self, event):
        """
        Implements the PySide2.QtWidgets.QWidget interface.

        event : See qt docs.
        """
        settings = qtc.QSettings()
        settings.setValue(self.__GEOMETRY_KEY,self.saveGeometry())
        settings.setValue(self.__STATE_KEY,self.__splitter.saveState())
        event.accept()


    #####################
    # PRIVATE - Methods #
    #####################


    def __setup_gui_(self):
        """
        Initializes the GUI of this new dialog.
        """
        layout = qtw.QVBoxLayout()
        layout.addWidget(self.__setup_view_())
        layout.addLayout(self.__setup_buttons_())
        self.setLayout(layout)
        self.setWindowTitle("Unknown Code Fragments")
        self.__restore_()


    def __setup_view_(self):
        """
        Initializes the GUI of this new dialog.
        """
        list_ = qtw.QListWidget()
        list_.currentTextChanged.connect(self.__item_changed_)
        list_.addItems(list(self.__code.keys()))
        self.__splitter.addWidget(list_)
        self.__splitter.addWidget(self.__view)
        return self.__splitter


    def __setup_buttons_(self):
        """
        Initializes the GUI of this new dialog.
        """
        close = qtw.QPushButton("Close")
        close.clicked.connect(self.__close_)
        copy = qtw.QPushButton("Copy to Clipboard")
        copy.clicked.connect(self.__copy_)
        ret = qtw.QHBoxLayout()
        ret.addWidget(close)
        ret.addStretch()
        ret.addWidget(copy)
        return ret


    def __restore_(self):
        """
        Restores the geometry of this dialog and the state of this dialog's qt splitter.
        """
        settings = qtc.QSettings()
        geometry = settings.value(self.__GEOMETRY_KEY)
        state = settings.value(self.__STATE_KEY)
        if geometry:
            self.restoreGeometry(geometry)
        if state:
            self.__splitter.restoreState(state)


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot(str)
    def __item_changed_(self, key):
        """
        Called to inform this dialog's the selected code fragment has changed to the one with the
        given key.

        key : The new key of the selected code fragment.
        """
        self.__view.setPlainText(self.__code[key])


    @qtc.Slot()
    def __copy_(self):
        """
        Called to copy this dialog's current code fragment to the system clipboard.
        """
        clipboard = qtg.QClipboard()
        clipboard.setText(self.__view.toPlainText())


    @qtc.Slot()
    def __close_(self):
        """
        Called to close this dialog.
        """
        self.done(qtw.QDialog.Accepted)
        self.close()


    #######################
    # PRIVATE - Constants #
    #######################


    #
    # The key used to save this dialog's geometry using qt settings to make it persistent.
    #
    __GEOMETRY_KEY = "gui.dialog.code.geometry"
    #
    # The key used to save this dialog's state using qt settings to make it persistent.
    #
    __STATE_KEY = "gui.dialog.code.state"
