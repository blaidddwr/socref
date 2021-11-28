"""
Contains the UnionBlock class.
"""
from .NamespaceBlock import NamespaceBlock
from PySide6.QtGui import QIcon
from socref import block




@block("Union")
class UnionBlock(NamespaceBlock):
    """
    This is the union block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ union.
    """


    def buildList(
        self
    ):
        return ()


    def icon(
        self
    ):
        return QIcon(":/socref_cpp/union.svg")


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "union"
