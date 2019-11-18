"""
todo
"""
import html
from PySide2 import QtGui as qtg
from socref import block_factory as bf
from socref import util
from . import package
from . import function
from . import access
from . import settings








class Block(package.Block):


    def __init__(self):
        package.Block.__init__(self)
        self._p_assignment = ""


    def is_argument(self):
        return isinstance(self.parent(),function.Block)


    def is_volatile_above(self):
        return self.is_argument()


    def in_class(self):
        return isinstance(self.parent(),access.Block)


    def icon(self):
        return qtg.QIcon(":/python/object.svg")


    def display_name(self):
        ret = self._p_name
        if self.is_argument() and self._p_assignment :
            ret += " ="
        return ret


    def build_list(self):
        return ()


    def argument_view(self):
        ret = "<p><b>%s " % html.escape(self._p_name)
        if self._p_assignment : ret += " =</b> " + html.escape(self._p_assignment) + " : "
        else: ret += "</b> : "
        ret += html.escape(self._p_description) + "</p>"
        return ret


    def display_view(self):
        assignment = html.escape(self._p_assignment)
        if assignment : assignment = "<h2>Assignment</h2><p>%s</p>" % assignment
        return package.Block.display_view(self) + assignment


    def edit_definitions(self):
        ret = package.Block.edit_definitions(self)
        ret.append(util.line_edit("Assignment:","_p_assignment"))
        return ret


    def set_default_properties(self):
        self._p_name = "object"
        self._p_description = "Detailed description."
        self._p_assignment = ""


    def clear_properties(self):
        self._p_name = ""
        self._p_description = ""
        self._p_assignment = ""








@bf.register_block("Object")
class Builder(Block):


    def argument(self):
        ret = self._p_name
        if self._p_assignment : ret += "=" + self._p_assignment
        return ret


    def comment(self, begin):
        initial = self._p_name + " : "
        return util.wrap_text(initial + self._p_description
                              ,begin=begin
                              ,after=" "*len(initial)
                              ,columns=settings.COLUMNS)


    def space(self, previous, above):
        if above._BLOCKNAME_ == "Module" :
            if previous is None : return "\n"*settings.H3LINES
            elif previous._BLOCKNAME_ == "Function" :
                return "\n"*settings.H2LINES
            elif previous._BLOCKNAME_ == "Class" :
                return "\n"*settings.H1LINES
            else: return ""
        elif above._BLOCKNAME_ == "Access" :
            if previous is None :
                return "\n"*settings.H3LINES
            elif previous._BLOCKNAME_ == "Function" :
                return "\n"*settings.H2LINES
            else:
                return ""
        else: raise RuntimeError("Unkonwn block type.")


    def build(self, def_, begin=""):
        ret = begin + "#\n"
        ret += util.wrap_text(self._p_description,begin=begin + "# ",columns=settings.COLUMNS)
        ret += begin + "#\n"
        ret += begin + self._p_name
        if self._p_assignment : ret += " = " + self._p_assignment
        ret += "\n"
        return ret
