"""
todo
"""
from PySide2.QtCore import Qt, Slot as QtSlot, QModelIndex
from PySide2.QtWidgets import QDockWidget, QWidget, QFormLayout, QHBoxLayout, QLineEdit, QPushButton
from .project_model import Project_Model






class Block_Edit_Dock(QDockWidget):


    def __init__(self, parent=None):
        QDockWidget.__init__(self,parent)
        #
        self.__view = None
        #
        self.__edits = []
        #:


    def set_view(self, view):
        self.__view = view
        self.__view.current_changed.connect(self.__current_changed_)


    @QtSlot(QModelIndex)
    def __current_changed_(self, index):
        if index.isValid() :
            self.setWindowTitle(self.__view.model().data(index,Qt.DisplayRole))
            self.setWidget(self.__build_form_widget(index))
        else :
            self.setWindowTitle("")
            self.__edits.clear()
            self.setWidget(None)


    @QtSlot()
    def __apply_(self):
        index = self.__view.selectionModel().currentIndex()
        if index.isValid() :
            props = {}
            for edit in self.__edits :
                props[edit._key] = edit._value_()
            self.__view.model().setData(index,props,Project_Model.PROPERTIES_ROLE)


    def __build_form_widget(self, index):
        self.__edits.clear()
        try:
            props = self.__view.model().data(index,Project_Model.PROPERTIES_ROLE)
            defs = self.__view.model().data(index,Project_Model.EDIT_DEFS_ROLE)
            layout = QFormLayout()
            for def_ in defs :
                edit = None
                if def_["type"] == "line" :
                    edit = QLineEdit(props[def_["key"]])
                    edit._value_ = lambda : edit.text()
                    edit._key = def_["key"]
                else : raise RuntimeError("Unknown edit definition.")
                layout.addRow(def_["label"],edit)
                self.__edits.append(edit)
            layout.addRow(self.__build_apply_())
            ret = QWidget()
            ret.setContentsMargins(0,16,0,4)
            ret.setLayout(layout)
            return ret
        except:
            self.__edits.clear()
            raise


    def __build_apply_(self):
        button = QPushButton("Apply")
        button.clicked.connect(self.__apply_)
        ret = QHBoxLayout()
        ret.addWidget(button)
        ret.addStretch()
        return ret
