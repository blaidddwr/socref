"""
Contains the EnumerationBlock class.
"""
from .NamespaceBlock import NamespaceBlock
from PySide6.QtGui import QIcon
from socref import block




@block("Enumeration")
class EnumerationBlock(NamespaceBlock):
    """
    This is the enumeration block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ enumeration.
    """


    def buildList(
        self
    ):
        return ()


    def icon(
        self
    ):
        return QIcon(":/socref_cpp/enumeration.svg")
