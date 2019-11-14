"""
todo
"""
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import project_model as pm






class Block_Edit_Dock(qtw.QDockWidget):


    def __init__(self, parent=None):
        qtw.QDockWidget.__init__(self,parent)
        self.__view = None
        self.__edits = []
        self.setWindowTitle("(Edit)")


    def set_view(self, view):
        self.__view = view
        self.__view.current_changed.connect(self.__current_changed_)


    @qtc.Slot(qtc.QModelIndex)
    def __current_changed_(self, index):
        if index.isValid() :
            model = self.__view.model()
            self.setWindowTitle("[%s] %s (Edit)" %
                                (model.data(index,pm.Project_Model.BLOCK_TYPE_ROLE)
                                 ,model.data(index,qtc.Qt.DisplayRole)))
            self.setWidget(self.__build_form_widget_(index))
        else:
            self.setWindowTitle("(Edit)")
            self.__edits.clear()
            self.setWidget(None)


    @qtc.Slot()
    def __apply_(self):
        index = self.__view.selectionModel().currentIndex()
        if index.isValid() :
            self.__view.model().setData(index
                                        ,{edit._key: edit._value_() for edit in self.__edits}
                                        ,pm.Project_Model.PROPERTIES_ROLE)


    def __build_form_widget_(self, index):
        self.__edits.clear()
        try:
            props = self.__view.model().data(index,pm.Project_Model.PROPERTIES_ROLE)
            defs = self.__view.model().data(index,pm.Project_Model.EDIT_DEFS_ROLE)
            layout = qtw.QFormLayout()
            for def_ in defs :
                edit = None
                label = ""
                if def_["type"] == "line" :
                    edit = qtw.QLineEdit(props[def_["key"]],self)
                    edit._value_ = lambda e=edit : e.text()
                    edit._key = def_["key"]
                    label = def_["label"]
                elif def_["type"] == "text" :
                    edit = qtw.QPlainTextEdit(props[def_["key"]],self)
                    edit._value_ = lambda e=edit : e.toPlainText()
                    edit._key = def_["key"]
                    label = def_["label"]
                elif def_["type"] == "checkbox" :
                    edit = qtw.QCheckBox(def_["label"],self)
                    edit.setCheckState(qtc.Qt.Checked if int(props[def_["key"]]) else qtc.Qt.Unchecked)
                    edit._value_ = lambda e=edit : str(int(e.checkState() == qtc.Qt.Checked))
                    edit._key = def_["key"]
                elif def_["type"] == "combobox" :
                    edit = qtw.QComboBox(self)
                    for selection in def_["selections"] :
                        if "icon" in selection : edit.addItem(selection["icon"],selection["text"])
                        else: edit.addItem(selection["text"])
                    edit.setCurrentText(props[def_["key"]])
                    edit._value_ = lambda e=edit : e.currentText()
                    edit._key = def_["key"]
                    label = def_["label"]
                else:
                    print(def_)
                    raise RuntimeError("Unknown edit definition.")
                layout.addRow(label,edit)
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
        button = qtw.QPushButton("Apply")
        button.clicked.connect(self.__apply_)
        ret = qtw.QHBoxLayout()
        ret.addWidget(button)
        ret.addStretch()
        return ret
