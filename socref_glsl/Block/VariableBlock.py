"""
Contains the VariableBlock class.
"""
from .ProgramBlock import ProgramBlock
from PySide6.QtGui import QIcon
from socref import block
from socref.Edit.LineEdit import LineEdit




@block("Variable")
class VariableBlock(ProgramBlock):
    """
    This is the variable block class. It implements the Socrates' Reference
    abstract block class. It represents a GLSL variable.
    """


    def __init__(
        self
    ):
        super().__init__()
        self._p_type = ""


    def buildList(
        self
    ):
        return ()


    def clearProperties(
        self
    ):
        super().clearProperties()
        self._p_type = ""


    def displayName(
        self
    ):
        return self._p_name


    def displayView(
        self
    ):
        ret = super().displayView()
        ret.addHeader("Type",1)
        ret.addText(self._p_type)
        return ret


    def editDefinitions(
        self
    ):
        ret = super().editDefinitions()
        ret.append(LineEdit("Type:","_p_type"))
        return ret


    def icon(
        self
    ):
        return QIcon(":/socref_glsl/variable.svg")


    def isVolatileAbove(
        self
    ):
        return True


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "variable"
        self._p_type = "float"
