"""
Contains the VariableBlock class.
"""
from .NamespaceBlock import NamespaceBlock
from PySide6.QtGui import QIcon
from socref import block
from socref.Edit.LineEdit import LineEdit




@block("Variable")
class VariableBlock(NamespaceBlock):
    """
    This is the variable block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ variable.
    """


    def __init__(
        self
    ):
        super().__init__()
        self._p_type = ""
        self._p_assignment = ""


    def buildList(
        self
    ):
        return ()


    def clearProperties(
        self
    ):
        super().clearProperties()
        self._p_type = ""
        self._p_assignment = ""


    def displayName(
        self
    ):
        flags = "".join(self.flags(FlagOutput.Compact))
        return "".join(
            (self._p_name," =" if self._p_assignment else ""," ["+flags+"]" if flags else "")
        )


    def displayView(
        self
    ):
        ret = super().displayView()
        ret.addHeader("Type",1)
        ret.addText(self._p_type)
        if self._p_assignment:
            ret.addHeader("Assignment",1)
            ret.addText(self._p_assignment)
        return ret


    def editDefinitions(
        self
    ):
        ret = super().editDefinitions()
        ret.append(LineEdit("Type:","_p_type"))
        ret.append(LineEdit("Assignment:","_p_assignment"))
        return ret


    def icon(
        self
    ):
        return QIcon(":/cpp/variable.svg")


    def isVolatileAbove(
        self
    ):
        return True


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "variable"
        self._p_type = "int"
