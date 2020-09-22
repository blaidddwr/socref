"""
Contains the EnumValue class.
"""
from ._object import Object
from PySide2 import QtGui as qtg
from socref import edit
from socref import register
from . import settings








@register("Enumeration_Value")
class EnumValue(Object):
    """
    This is the enumeration value block class. It implements the Socrates'
    Reference abstract block class. It represents a python enumeration value
    object. The standard python library is used for the generated class as an
    integer enumeration. If no value is given the auto class is used from the
    python enumeration library.
    """


    def __init__(
        self
        ):
        """
        Initializes a new enumeration block.
        """
        Object.__init__(self)


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
        ret = [begin+"#"]
        ret += edit.wrapBlocks(self._p_description,begin=begin + "# ",columns=settings.COLUMNS)
        line = begin+self._p_name
        if self._p_assignment:
            line += " = " + self._p_assignment
        else:
            line += " = enum.auto()"
        ret.append(line)
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
        return qtg.QIcon(":/python/enumeration_value.svg")


    def isArgument(
        self
        ):
        """
        Implements the socref_python.block.Object interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return False


    def inClass(
        self
        ):
        """
        Implements the socref_python.block.Object interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return True


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Object.setDefaultProperties(self)
        self._p_name = "enumeration_value"
