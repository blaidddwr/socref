"""
Detailed description.
"""
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import settings
from . import model
from . import gui_util








class Plain_Text(qtw.QPlainTextEdit):
    """
    Detailed description.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, text="", speller=False, parent=None):
        """
        Detailed description.

        text : Detailed description.

        speller : Detailed description.

        parent : Detailed description.
        """
        qtw.QPlainTextEdit.__init__(self,text,parent)
        self.__highlighter = None
        if speller : self.set_speller_enabled(True)


    ####################
    # PUBLIC - Methods #
    ####################


    def set_speller_enabled(self, enabled):
        """
        Detailed description.

        enabled : Detailed description.
        """
        if not enabled :
            self.__highlighter.deleteLater()
            self.__highlighter = None
        elif self.__highlighter is None :
            self.__highlighter = gui_util.Spell_Highlighter(settings.DICTIONARY,self.document())








class Block(qtw.QDockWidget):
    """
    Detailed description.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, parent=None):
        """
        Detailed description.

        parent : Detailed description.
        """
        qtw.QDockWidget.__init__(self,parent)
        self.__view = None
        self.__edits = []
        self.setWindowTitle("(Edit)")


    ####################
    # PUBLIC - Methods #
    ####################


    def set_view(self, view):
        """
        Detailed description.

        view : Detailed description.
        """
        self.__view = view
        self.__view.current_changed.connect(self.__current_changed_)


    #####################
    # PRIVATE - Methods #
    #####################


    def __build_form_widget_(self, index):
        """
        Detailed description.

        index : Detailed description.
        """
        self.__edits.clear()
        try:
            props = self.__view.model().data(index,model.Role.PROPERTIES)
            defs = self.__view.model().data(index,model.Role.EDIT_DEFS)
            layout = qtw.QFormLayout()
            for def_ in defs :
                edit = None
                label = ""
                hidden = False
                if def_["type"] == "line" :
                    edit = qtw.QLineEdit(props[def_["key"]])
                    edit._value_ = lambda e=edit : e.text()
                    edit._key = def_["key"]
                    label = def_["label"]
                elif def_["type"] == "text" :
                    edit = Plain_Text(props[def_["key"]],def_.get("speller",False))
                    edit._value_ = lambda e=edit : e.toPlainText()
                    edit._key = def_["key"]
                    label = def_["label"]
                elif def_["type"] == "checkbox" :
                    edit = qtw.QCheckBox(def_["label"])
                    edit.setCheckState(qtc.Qt.Checked if int(props[def_["key"]]) else qtc.Qt.Unchecked)
                    edit._value_ = lambda e=edit : str(int(e.checkState() == qtc.Qt.Checked))
                    edit._key = def_["key"]
                elif def_["type"] == "combobox" :
                    edit = qtw.QComboBox()
                    for selection in def_["selections"] :
                        if "icon" in selection : edit.addItem(selection["icon"],selection["text"])
                        else: edit.addItem(selection["text"])
                    edit.setCurrentText(props[def_["key"]])
                    edit._value_ = lambda e=edit : e.currentText()
                    edit._key = def_["key"]
                    label = def_["label"]
                elif def_["type"] == "hidden" :
                    edit = qtw.QWidget()
                    edit._value_ = lambda val=def_["value"] : val
                    edit._key = def_["key"]
                    hidden = True
                else:
                    print(def_)
                    raise RuntimeError("Unknown edit definition.")
                if not hidden : layout.addRow(label,edit)
                self.__edits.append(edit)
            layout.addRow(self.__build_apply_())
            ret = qtw.QWidget()
            ret.setContentsMargins(0,16,0,4)
            ret.setLayout(layout)
            return ret
        except:
            self.__edits.clear()
            raise


    def __build_apply_(self):
        """
        Detailed description.

        return : Yes
        """
        button = qtw.QPushButton("Apply")
        button.clicked.connect(self.__apply_)
        ret = qtw.QHBoxLayout()
        ret.addWidget(button)
        ret.addStretch()
        return ret


    ###################
    # PRIVATE - Slots #
    ###################


    @qtc.Slot(qtc.QModelIndex)
    def __current_changed_(self, index):
        """
        Detailed description.

        index : Detailed description.
        """
        if index.isValid() :
            m = self.__view.model()
            self.setWindowTitle("[%s] %s (Edit)" %
                                (m.data(index,model.Role.BLOCK_TYPE)
                                 ,m.data(index,qtc.Qt.DisplayRole)))
            self.setWidget(self.__build_form_widget_(index))
        else:
            self.setWindowTitle("(Edit)")
            self.__edits.clear()
            self.setWidget(None)


    @qtc.Slot()
    def __apply_(self):
        """
        Detailed description.
        """
        index = self.__view.selectionModel().currentIndex()
        if index.isValid() :
            self.__view.model().setData(index
                                        ,{edit._key: edit._value_() for edit in self.__edits}
                                        ,model.Role.PROPERTIES)
