"""
Contains the NamespaceBlock class.
"""
from PySide6.QtGui import QIcon
from socref import block
from socref.Base.BlockBase import BlockBase
from socref.Edit.LineEdit import LineEdit
from socref.Edit.TextEdit import TextEdit
from socref.Output.RichText import RichText




@block("Namespace",root=True)
class NamespaceBlock(BlockBase):
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
        return ("Namespace","Enumeration","Union","Class","Function")


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


    def hasFunctions(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this namespace block has one or more functions or false
                 otherwise.
        """
        for child in self:
            if child._TYPE_ == "Function":
                return True
        return False


    def icon(
        self
    ):
        return QIcon(":/cpp/namespace.svg")


    def setDefaultProperties(
        self
    ):
        self._p_description = "Detailed description."
