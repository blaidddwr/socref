"""
Detailed description.
"""
import html
from PySide2 import QtGui as qtg
from socref import util
from socref import abstract
from . import package
from . import function
from . import access
from . import settings








class Block(package.Block):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self):
        """
        Detailed description.
        """
        package.Block.__init__(self)
        self._p_assignment = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def is_argument(self):
        """
        Detailed description.

        return : Yes
        """
        return isinstance(self.parent(),function.Block)


    def is_volatile_above(self):
        """
        Detailed description.

        return : Yes
        """
        return self.is_argument()


    def in_class(self):
        """
        Detailed description.

        return : Yes
        """
        return isinstance(self.parent(),access.Block)


    def icon(self):
        """
        Detailed description.

        return : Yes
        """
        return qtg.QIcon(":/python/object.svg")


    def display_name(self):
        """
        Detailed description.

        return : Yes
        """
        ret = self._p_name
        if self.is_argument() and self._p_assignment :
            ret += " ="
        return ret


    def display_view(self):
        """
        Detailed description.

        return : Yes
        """
        assignment = html.escape(self._p_assignment)
        if assignment : assignment = "<h2>Assignment</h2><p>%s</p>" % assignment
        return package.Block.display_view(self) + assignment


    def argument_view(self):
        """
        Detailed description.

        return : Yes
        """
        ret = "<p><b>%s " % html.escape(self._p_name)
        if self._p_assignment : ret += " =</b> " + html.escape(self._p_assignment) + " : "
        else: ret += "</b> : "
        ret += html.escape(self._p_description) + "</p>"
        return ret


    def build_list(self):
        """
        Detailed description.

        return : Yes
        """
        return ()


    def edit_definitions(self):
        """
        Detailed description.

        return : Yes
        """
        ret = package.Block.edit_definitions(self)
        ret.append(util.line_edit("Assignment:","_p_assignment"))
        return ret


    def set_default_properties(self):
        """
        Detailed description.
        """
        package.Block.set_default_properties(self)
        self._p_name = "object"
        self._p_assignment = ""


    def clear_properties(self):
        """
        Detailed description.
        """
        package.Block.clear_properties(self)
        self._p_assignment = ""








@abstract.register_block("Object")
class Builder(Block):
    """
    Detailed description.
    """


    ####################
    # PUBLIC - Methods #
    ####################


    def argument(self):
        """
        Detailed description.

        return : Yes
        """
        ret = self._p_name
        if self._p_assignment : ret += "=" + self._p_assignment
        return ret


    def comment(self, begin):
        """
        Detailed description.

        begin : Detailed description.

        return : Yes
        """
        initial = self._p_name + " : "
        return util.wrap_text(initial + self._p_description
                              ,begin=begin
                              ,after=" "*len(initial)
                              ,columns=settings.COLUMNS)


    def space(self, previous, above):
        """
        Detailed description.

        previous : Detailed description.

        above : Detailed description.

        return : Yes
        """
        if above._TYPE_ == "Module" :
            if previous is None : return "\n"*settings.H3LINES
            elif previous._TYPE_ == "Function" :
                return "\n"*settings.H2LINES
            elif previous._TYPE_ == "Class" :
                return "\n"*settings.H1LINES
            else: return ""
        elif above._TYPE_ == "Access" :
            if previous is None :
                return "\n"*settings.H3LINES
            elif previous._TYPE_ == "Function" :
                return "\n"*settings.H2LINES
            else:
                return ""
        else: raise RuntimeError("Unkonwn block type.")


    def build(self, def_, begin=""):
        """
        Detailed description.

        def_ : Detailed description.

        begin : Detailed description.

        return : Yes
        """
        ret = begin + "#\n"
        ret += util.wrap_blocks(self._p_description,begin=begin + "# ",columns=settings.COLUMNS)
        ret += begin + "#\n"
        ret += begin + self._p_name
        if self._p_assignment : ret += " = " + self._p_assignment
        ret += "\n"
        return ret
