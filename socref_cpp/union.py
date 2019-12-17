"""
Contains the union block class.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import settings
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


    def buildDeclaration(self, begin):
        """
        Implements the .namespace.Base interface.

        begin : See interface docs.

        return : See interface docs.
        """
        ret = [""]*settings.H2LINES
        ret.append(begin+"/*!")
        ret += ut.wrapText(self._p_description,begin+" * ",columns=settings.COLUMNS)
        ret.append(begin+" */")
        ret.append(begin+"union "+self._p_name)
        ret.append(begin+"{")
        nextBegin = begin + " "*settings.INDENT
        for child in self:
            ret += child.buildDeclaration(nextBegin,False)
        ret.append(begin+"};")
        return ret


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Variable",)


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/cpp/union.svg")


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.setDefaultProperties(self)
        self._p_name = "union"
