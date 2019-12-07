"""
Contains the namespace block definition and generic base block.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from socref import abstract
from . import parser








class Base(abstract.Block):
    """
    This is the base block class. It implements the Socrates' Reference abstract block class. It is
    a base block class the many other C++ blocks use which contains name and description properties.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new namespace block.
        """
        abstract.Block.__init__(self)
        self._p_name = ""
        self._p_description = ""


    ####################
    # PUBLIC - Methods #
    ####################


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
        return ut.rich_text_blocks(1,"Description",html.escape(self._p_description))


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = []
        ret.append(ut.line_edit("Name:","_p_name"))
        ret.append(ut.text_edit("Description:","_p_description",speller=True))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        self._p_description = "Detailed Description."


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        self._p_name = ""
        self._p_description = ""








@register("Namespace",root=True)
class Block(Base):
    """
    This is the namespace block class. It implements the Socrates' Reference abstract block class.
    It represents a C++ namespace. It is the root block type of a C++ project.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new namespace block.
        """
        Base.__init__(self)
        self._p_hidden = "0"


    ####################
    # PUBLIC - Methods #
    ####################


    def is_hidden(self):
        """
        Getter method.

        return : True if this is a hidden namespace that must not have its name added to the scoping
                 of source code files nor have its own header or source file. If this is a regular
                 visible namespace then false is returned.
        """
        return bool(int(self._p_hidden))


    def has_functions(self):
        """
        Getter method.

        return : True if this namespace has one or more functions or false otherwise.
        """
        for child in self:
            if child._TYPE_ == "Function":
                return True
        return False


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        if self.is_hidden():
            return qtg.QIcon(":/cpp/hidden_namespace.svg")
        else:
            return qtg.QIcon(":/cpp/namespace.svg")


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ("Namespace","Enumeration","Variable","Function","Class","Union")


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = Base.edit_definitions(self)
        ret.append(ut.checkbox_edit("Hidden","_p_hidden"))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        super().set_default_properties(self)
        self._p_name = "namespace"
        self._p_hidden = "0"


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        Base.clear_properties(self)
        self._p_hidden = "0"


    def parser(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return parser.Parser(self)
