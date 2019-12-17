"""
Contains the enumeration block class.
"""
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import settings
from . import namespace








@register("Enumeration")
class Enum(namespace.Base):
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
        namespace.Base.__init__(self)
        self._p_class = "0"


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
        line = begin+"enum "
        if self.isClass():
            line += "class "
        line += self._p_name
        ret.append(line)
        ret.append(begin+"{")
        nextBegin = begin + " "*settings.INDENT
        first = True
        for child in self:
            ret += child.buildDeclaration(nextBegin,first)
            if first:
                first = False
        ret.append(begin+"}")
        return ret


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Enumeration Value",)


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.clearProperties(self)
        self._p_class = "0"


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = self._p_name
        if self.isClass():
            ret += " [C]"
        return ret


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        class_ = "<p><i>Class</i></p>" if self.isClass() else ""
        return namespace.Base.displayView(self) + class_


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = namespace.Base.editDefinitions(self)
        ret.append(ut.checkboxEdit("Class","_p_class"))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/cpp/enumeration.svg")


    def isClass(self):
        """
        Getter method.

        return : True if this is a class enumeration or false if it is a regular enumeration.
        """
        return bool(int(self._p_class))


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.setDefaultProperties(self)
        self._p_name = "enumeration"
        self._p_class = "0"
