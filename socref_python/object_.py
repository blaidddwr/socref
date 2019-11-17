"""
todo
"""
import html
from PySide2 import QtGui as qtg
from socref import block_factory as bf
from . import package
from . import function
from . import access






@bf.register_block("Object")
class Block(package.Block):


    def __init__(self):
        package.Block.__init__(self)
        #
        self._p_assignment = ""
        #
        self._p_static = "0"
        #:


    def is_argument(self):
        return isinstance(self.parent(),function.Block)


    def is_volatile_above(self):
        return self.is_argument()


    def is_static(self):
        return bool(int(self._p_static))


    def in_class(self):
        return isinstance(self.parent(),access.Block)


    def icon(self):
        if not self.is_static() : return qtg.QIcon(":/python/object.svg")
        else: return qtg.QIcon(":/python/static_object.svg")


    def display_name(self):
        ret = self._p_name
        if self.is_argument() and self._p_assignment :
            ret += " ="
        return ret


    def build_list(self):
        return ()


    def display_view(self):
        self.__check_flags_()
        assignment = html.escape(self._p_assignment)
        if assignment : assignment = "<h2>Assignment</h2><p>%s</p>" % assignment
        static = "<h3>Static</h3>" if self.is_static() else ""
        return package.Block.display_view(self) + assignment + static


    def edit_definitions(self):
        ret = package.Block.edit_definitions(self)
        ret.append(self._line_edit_("Assignment:","_p_assignment"))
        if self.in_class() : ret.append(self._checkbox_edit_("Static","_p_static"))
        return ret


    def set_default_properties(self):
        self._p_name = "object"
        self._p_description = "Detailed description."
        self._p_assignment = ""
        self._p_static = "0"


    def clear_properties(self):
        self._p_name = ""
        self._p_description = ""
        self._p_assignment = ""
        self._p_static = "0"


    def argument(self):
        ret = self._p_name
        if self._p_assignment : ret += "=" + self._p_assignment
        return ret


    def comment(self, begin):
        return begin + "YOLO\n"


    def __check_flags_(self):
        if not self.in_class() : self._p_static = "0"
