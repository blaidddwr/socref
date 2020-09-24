"""
Contains the Union class.
"""
from PySide2 import QtGui as qtg
from socref import edit
from socref import register
from . import settings
from ._base import Base








@register("Union")
class Union(Base):
    """
    This is the union block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ union.
    """


    def __init__(
        self
        ):
        """
        Initializes a new union block.
        """
        super().__init__()


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
        ret.append(begin+"union "+self._p_name)
        ret.append(begin+"{")
        nextBegin = begin + " "*settings.INDENT
        for child in self:
            ret += child.buildDeclaration(nextBegin)
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
        return ("Variable",)


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
        return qtg.QIcon(":/cpp/union.svg")


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.setDefaultProperties(self)
        self._p_name = "union"
