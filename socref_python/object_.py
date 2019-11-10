"""
todo
"""
import html
from PySide2 import QtGui as qtg
from socref import block_factory as bf
from . import package
from . import function






@bf.register_block("Object")
class Block(package.Block):


    def __init__(self):
        package.Block.__init__(self)
        #:


    def assignment(self, escape=True):
        ret = self._props["assignment"]
        if escape : ret = html.escape(ret)
        return ret


    def is_argument(self):
        return isinstance(self.parent(),function.Block)


    def is_volatile_above(self):
        return self.is_argument()


    def icon(self):
        return qtg.QIcon(":/python/object.svg")


    def display_name(self):
        ret = package.Block.display_name(self)
        if self.is_argument() and self.assignment() :
            ret += " ="
        return ret


    def build_list(self):
        return ()


    def display_view(self):
        assignment = self.assignment(escape=True)
        if assignment : assignment = "<h2>Assignment</h2><p>%s</p>" % assignment
        return package.Block.display_view(self) + assignment


    def edit_definitions(self):
        ret = package.Block.edit_definitions(self)
        element = dict()
        element["type"] = "text"
        element["label"] = "Assignment:"
        element["key"] = "assignment"
        ret.append(element)
        return ret


    def set_default_properties(self):
        self._props["name"] = "object"
        self._props["assignment"] = ""
        self._props["description"] = "Detailed description."


    def clear_properties(self):
        self._props["name"] = ""
        self._props["assignment"] = ""
        self._props["description"] = ""
