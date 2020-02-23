"""
Contains the enumeration value block class.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import settings
from . import namespace








@register("Enumeration_Value")
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


    def buildDeclaration(self, begin, first):
        """
        Implements the .namespace.Base interface with one additional argument.

        Parameters
        ----------
        begin : object
                See interface docs.
        first : bool
                True if this is the first value in its enumeration parent or false otherwise.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = ut.wrapText(self._p_description,begin+"/// ",columns=settings.COLUMNS)
        line = begin+("" if first else ",")+self._p_name
        if self._p_value:
            line += " = "+self._p_value
        ret.append(line)
        return ret


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
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

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        value = ut.richText(2,"Value",html.escape(self._p_value))
        return namespace.Base.displayView(self) + value


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = namespace.Base.editDefinitions(self)
        ret.append(ut.lineEdit("Value:","_p_value"))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return qtg.QIcon(":/cpp/enumeration_value.svg")


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.setDefaultProperties(self)
        self._p_name = "enumeration_value"
        self._p_value = ""
