"""
Contains the Namespace class.
"""
from PySide6.QtGui import QIcon
from socref import block
from socref.Base.BlockBase import BlockBase
from socref.Edit.LineEdit import LineEdit
from socref.Edit.TextEdit import TextEdit
from socref.Output.RichText import RichText




@block("Namespace",root=True)
class Namespace(BlockBase):
    """
    This is the namespace block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ namespace.
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
        return ("Namespace",)#"Enumeration","Variable","Function","Class","Union")


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
        return qtg.QIcon(":/cpp/namespace.svg")


    def setDefaultProperties(
        self
    ):
        self._p_description = "Detailed description."
