"""
Contains the enumeration value block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import namespace








@register("Enumeration Value")
class EnumValue(namespace.Base):
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


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ()


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.clearProperties(self)
        self._p_value = ""


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        value = ut.richText(2,"Value",html.escape(self._p_value))
        return namespace.Base.displayView(self) + value


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = namespace.Base.editDefinitions(self)
        ret.append(ut.lineEdit("Value:","_p_value"))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/cpp/enumeration_value.svg")


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.setDefaultProperties(self)
        self._p_name = "enumeration_value"
        self._p_value = "value"
