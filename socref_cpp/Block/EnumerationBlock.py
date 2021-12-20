"""
Contains the EnumerationBlock class.
"""
from .NamespaceBlock import NamespaceBlock
from PySide6.QtGui import QIcon
from socref import block
from socref.Edit.CheckBoxEdit import CheckBoxEdit




@block("Enumeration")
class EnumerationBlock(NamespaceBlock):
    """
    This is the enumeration block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ enumeration.
    """


    def __init__(
        self
    ):
        super().__init__()
        self._p_class = "0"


    def buildList(
        self
    ):
        return ()


    def clearProperties(
        self
    ):
        super().clearProperties()
        self._p_class = "0"


    def displayName(
        self
    ):
        return self._p_name+" [C]" if self.isClass() else self._p_name


    def editDefinitions(
        self
    ):
        ret = super().editDefinitions()
        ret.append(CheckBoxEdit("Class","_p_class"))
        return ret


    def icon(
        self
    ):
        return QIcon(":/socref_cpp/enumeration.svg")


    def isClass(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this is a class enumeration or false otherwise.
        """
        return bool(int(self._p_class))


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "enumeration"
        self._p_class = "0"
