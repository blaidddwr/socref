"""
Contains the Enum class.
"""
from PySide2 import QtGui as qtg
from socref import edit
from socref import register
from . import settings
from ._base import Base








@register("Enumeration")
class Enum(Base):
    """
    This is the enumeration block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ enumeration.
    """


    def __init__(
        self
        ):
        """
        Initializes a new enumeration block.
        """
        Base.__init__(self)
        self._p_class = "0"


    def buildDeclaration(
        self
        ,begin
        ):
        """
        Implements the socref_cpp.block.Base interface.

        Parameters
        ----------
        begin : object
                See interface docs.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = [""]*settings.H2LINES
        ret.append(begin+"/*!")
        ret += edit.wrapText(self._p_description,begin+" * ",columns=settings.COLUMNS)
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
        ret.append(begin+"};")
        return ret


    def buildList(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return ("Enumeration_Value",)


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.clearProperties(self)
        self._p_class = "0"


    def displayName(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = self._p_name
        if self.isClass():
            ret += " [C]"
        return ret


    def displayView(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        class_ = "<p><i>Class</i></p>" if self.isClass() else ""
        return Base.displayView(self) + class_


    def editDefinitions(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = Base.editDefinitions(self)
        ret.append(edit.checkboxEdit("Class","_p_class"))
        return ret


    def icon(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return qtg.QIcon(":/cpp/enumeration.svg")


    def isClass(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this is a class enumeration or false if it is a regular
               enumeration.
        """
        return bool(int(self._p_class))


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.setDefaultProperties(self)
        self._p_name = "enumeration"
        self._p_class = "0"
