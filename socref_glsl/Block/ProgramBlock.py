"""
Contains the ProgramBlock class.
"""
from PySide6.QtGui import QIcon
from socref import block
from socref.Base.BlockBase import BlockBase
from socref.Edit.LineEdit import LineEdit
from socref.Edit.TextEdit import TextEdit
from socref.Output.RichText import RichText




@block("Program",root=True)
class ProgramBlock(BlockBase):
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
        return ("Program","Shader")


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
        ret = RichText()
        ret.addHeader("Description",1)
        ret.addText(self._p_description)
        return ret


    def editDefinitions(
        self
    ):
        return [LineEdit("Name:","_p_name"),TextEdit("Description:","_p_description",True)]


    def icon(
        self
    ):
        return QIcon(":/socref_glsl/program.svg")


    def key(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : string
                 Unique key used to obtain the correct reader for the writer of
                 this block.
        """
        names = []
        b = self
        while b is not None:
            if b._p_name:
                names.append(b._p_name)
            b = b.parent()
        names.reverse()
        return ".".join(names)


    def setDefaultProperties(
        self
    ):
        self._p_name = "program"
        self._p_description = "Detailed description."
