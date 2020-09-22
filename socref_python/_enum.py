"""
Contains the Enum class.
"""
from ._package import Package
from PySide2 import QtGui as qtg
from socref import edit
from socref import register
from . import settings








@register("Enumeration")
class Enum(Package):
    """
    This is the enumeration block class. It implements the Socrates' Reference
    abstract block class. It represents a python enumeration class. The standard
    python library is used for the generated class as an integer enumeration.
    """


    def __init__(
        self
        ):
        """
        Initializes a new enumeration block.
        """
        Package.__init__(self)


    def build(
        self
        ,definition
        ,begin=""
        ):
        """
        Implements the socref_python.block.Package interface.

        Parameters
        ----------
        definition : object
                     See interface docs.
        begin : object
                See interface docs.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = [""]*settings.H1LINES
        ret.append("%sclass %s(enum.IntEnum):" % (begin,self._p_name))
        newBegin = begin + " "*settings.INDENT
        ret.append(newBegin+'"""')
        ret += edit.wrapBlocks(self._p_description,begin=newBegin,columns=settings.COLUMNS)
        ret.append(newBegin+'"""')
        (regular,classes) = self._buildChildren_(definition,newBegin)
        ret += regular
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
        return qtg.QIcon(":/python/enumeration.svg")


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Package.setDefaultProperties(self)
        self._p_name = "enumeration"
