"""
Contains the VariableBlock class.
"""
from .ProgramBlock import *
from PySide6.QtGui import QIcon
from socref import block
from socref.Edit.LineEdit import *




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
        if self._p_name:
            return self._p_name
        else:
            return self._p_type.split()[-1]


    def displayView(
        self
    ):
        ret = super().displayView()
        ret.addHeader("Type",1)
        ret.addText(self._p_type.replace("@",self._p_name))
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


    def inStructure(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this variable is part of a structure or false
                 otherwise.
        """
        return self.parent() and self.parent()._TYPE_ == "Structure"


    def isArgument(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this variable is an argument of a function or false
                 otherwise.
        """
        return self.parent() and self.parent()._TYPE_ == "Function"


    def isVolatileAbove(
        self
    ):
        return self.isArgument()


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "variable"
        self._p_type = "float @"
