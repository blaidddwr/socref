"""
Contains the ProgramBlock class.
"""
from PySide2 import QtGui as qtg
from socref import block
from socref.Abstract.AbstractBlock import *
from socref.Edit.LineEdit import *
from socref.Edit.TextEdit import *
from socref.Output.RichText import *




@block("Program",root=True)
class ProgramBlock(AbstractBlock):
    """
    This is the program block class. It implements the Socrates' Reference
    abstract block class. It represents a GLSL program, which is nothing more
    than a folder in the project's file system. It is the root block type of a
    GLSL project. Each folder contains a README text file that contains the
    program block's description.
    """


    def __init__(
        self
    ):
        super().__init__()
        self._p_name = ""
        self._p_description = ""


    def buildList(
        self
    ):
        return ("Program",)


    def clearProperties(
        self
    ):
        self._p_name = ""
        self._p_description = ""


    def displayName(
        self
    ):
        return self._p_name


    def displayView(
        self
    ):
        rt = RichText()
        rt.addHeader("Description",1)
        rt.addText(self._p_description)
        return rt


    def editDefinitions(
        self
    ):
        return [LineEdit("Name:","_p_name"),TextEdit("Description:","_p_description",True)]


    def icon(
        self
    ):
        return qtg.QIcon(":/socref_glsl/program.svg")


    def setDefaultProperties(
        self
    ):
        self._p_name = "program"
        self._p_description = "Detailed description."
