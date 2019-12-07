"""
Contains the namespace block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from socref import abstract








@register("Namespace",root=True)
class Block(abstract.Block):
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
        abstract.Block.__init__(self)
        self._p_name = ""
        self._p_description = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/cpp/namespace.svg")


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
        ret = []
        ret.append(ut.line_edit("Name:","_p_name"))
        ret.append(ut.text_edit("Description:","_p_description",speller=True))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        self._p_name = "namespace"
        self._p_description = "Detailed Description."


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        self._p_name = ""
        self._p_description = ""
