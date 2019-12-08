"""
Contains the enumeration block definition.
"""
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
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


    def is_class(self):
        """
        Getter method.

        return : True if this is a class enumeration or false if it is a regular enumeration.
        """
        return bool(int(self._p_class))


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/cpp/enumeration.svg")


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = self._p_name
        if self.is_class():
            ret += " [C]"
        return ret


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        class_ = "<p><i>Class</i></p>" if self.is_class() else ""
        return namespace.Base.displayView(self) + class_


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Enumeration Value",)


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = namespace.Base.editDefinitions(self)
        ret.append(ut.checkboxEdit("Class","_p_class"))
        return ret


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.setDefaultProperties(self)
        self._p_name = "enumeration"
        self._p_class = "0"


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.clearProperties(self)
        self._p_class = "0"
