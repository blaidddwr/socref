"""
Contains the enumeration value block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import namespace








@register("Enumeration Value")
class Block(namespace.Block):
    """
    This is the enumeration value block class. It implements the Socrates' Reference abstract block
    class. It represents a C++ enumeration value.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new enumeration value block.
        """
        namespace.Block.__init__(self)
        self._p_value = ""


    ##########################
    # PUBLIC - Basic Methods #
    ##########################


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/cpp/enumeration_value.svg")


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        value = ut.rich_text(2,"Value",html.escape(self._p_value))
        return namespace.Block.display_view(self) + value


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ()


    #############################
    # PUBLIC - Property Methods #
    #############################


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = namespace.Block.edit_definitions(self)
        ret.append(ut.line_edit("Value:","_p_value"))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        namespace.Block.set_default_properties(self)
        self._p_name = "enumeration_value"
        self._p_value = "value"


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        namespace.Block.clear_properties(self)
        self._p_value = ""
