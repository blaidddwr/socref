"""
Contains the class block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import namespace
from . import function








@register("Class")
class Block(function.Templatee):
    """
    This is the class block class. It implements the Socrates' Reference abstract block class. It
    represents a C++ class.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new class block.
        """
        function.Templatee.__init__(self)
        self._p_header = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def is_abstract(self):
        """
        Getter method.

        return : True if this class contains any abstract functions or false otherwise.
        """
        for child in self:
            if child._TYPE_ == "Access" and child.has_abstract():
                return True
        return False


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        if self.is_abstract():
            return qtg.QIcon(":/cpp/abstract_class.svg")
        else:
            return qtg.QIcon(":/cpp/class.svg")


    def display_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return self._template_name_() + self._p_name


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return namespace.Block.display_view(self) + self._templates_view_()


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ("Template","Access")


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = namespace.Block.edit_definitions(self)
        ret.append(ut.text_edit("Header:","_p_header"))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        namespace.Block.set_default_properties(self)
        self._p_name = "class"
        self._p_header = ""


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        namespace.Block.clear_properties(self)
        self._p_header = ""
