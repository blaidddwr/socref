"""
Contains the enumeration value block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import namespace








@register("Enumeration Value")
class Block(namespace.Base):
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
        namespace.Base.__init__(self)
        self._p_value = ""


    ####################
    # PUBLIC - Methods #
    ####################


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
        return namespace.Base.display_view(self) + value


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ()


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = namespace.Base.edit_definitions(self)
        ret.append(ut.line_edit("Value:","_p_value"))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        namespace.Base.set_default_properties(self)
        self._p_name = "enumeration_value"
        self._p_value = "value"


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        namespace.Base.clear_properties(self)
        self._p_value = ""
