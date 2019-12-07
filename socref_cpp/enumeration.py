"""
Contains the enumeration block definition.
"""
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import namespace








@register("Enumeration")
class Block(namespace.Block):
    """
    This is the enumeration block class. It implements the Socrates' Reference abstract block class.
    It represents a C++ enumeration.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new enumeration block.
        """
        namespace.Block.__init__(self)
        self._p_class = "0"


    ####################
    # PUBLIC - Methods #
    ####################


    def is_class(self):
        """
        Getter method.

        return : True if this is a class enumeration or false if it is a regular enumeration.
        """
        return bool(int(self._p_class))


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/cpp/enumeration.svg")


    def display_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = self._p_name
        if self.is_class():
            ret += " [C]"
        return ret


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        class_ = "<p><i>Class</i></p>" if self.is_class() else ""
        return namespace.Block.display_view(self) + class_


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ("Enumeration Value",)


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = namespace.Block.edit_definitions(self)
        ret.append(ut.checkbox_edit("Class","_p_class"))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        namespace.Block.set_default_properties(self)
        self._p_name = "enumeration"
        self._p_class = "0"


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        namespace.Block.clear_properties(self)
        self._p_class = "0"
