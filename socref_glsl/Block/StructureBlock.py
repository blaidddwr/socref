"""
Contains the StructureBlock class.
"""
from .ProgramBlock import *
from PySide2 import QtGui as qtg
from socref import block
from socref.Edit.LineEdit import *




@block("Structure")
class StructureBlock(ProgramBlock):
    """
    This is the structure block class. It implements the Socrates' Reference
    abstract block class. It represents a GLSL structure.
    """


    def __init__(
        self
    ):
        super().__init__()
        self._p_type = ""


    def buildList(
        self
    ):
        return ("Variable",)


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
        return qtg.QIcon(":/socref_glsl/structure.svg")


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "structure"
        self._p_type = "struct Struct"
