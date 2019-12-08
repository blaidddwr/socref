"""
Contains the union block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import namespace








@register("Union")
class Union(namespace.Base):
    """
    This is the union block class. It implements the Socrates' Reference abstract block class. It
    represents a C++ union.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new union block.
        """
        namespace.Base.__init__(self)


    ####################
    # PUBLIC - Methods #
    ####################


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/cpp/union.svg")


    def build_list(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Variable",)


    def set_default_properties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.set_default_properties(self)
        self._p_name = "union"
