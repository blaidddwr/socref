"""
Contains the template block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import namespace








@register("Template")
class Block(namespace.Block):
    """
    This is the template block class. It implements the Socrates' Reference abstract block class. It
    represents a C++ template.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new namespace block.
        """
        namespace.Block.__init__(self)
        self._p_type = ""
        self._p_assignment = ""


    ##########################
    # PUBLIC - Basic Methods #
    ##########################


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/cpp/template.svg")


    def display_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return self._p_name


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        type_ = ut.rich_text(2,"Type",html.escape(self._p_type))
        assignment = ut.rich_text(2,"Assignment",html.escape(self._p_assignment))
        return namespace.Block.display_view(self) + type_ + assignment


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ()


    def is_volatile_above(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return True


    #############################
    # PUBLIC - Property Methods #
    #############################


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = namespace.Block.edit_definitions(self)
        ret.append(ut.line_edit("Type:","_p_type"))
        ret.append(ut.line_edit("Assignment:","_p_assignment"))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        namespace.Block.set_default_properties(self)
        self._p_name = "Template"
        self._p_type = "class @"
        self._p_assignment = ""


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        namespace.Block.clear_properties(self)
        self._p_name = ""
        self._p_type = ""
        self._p_assignment = ""
