"""
Contains the ObjectBlock class.
"""
from .ModuleBlock import *
from PySide2.QtGui import QIcon
from socref import block
from socref.Edit.LineEdit import *
from socref.Output.RichText import *




@block("Object")
class ObjectBlock(ModuleBlock):
    """
    This is the object block class. It implements the Socrates' Reference
    abstract block class. It represents a python object.
    """


    def __init__(
        self
    ):
        super().__init__()
        self._p_assignment = ""


    def buildList(
        self
    ):
        return ()


    def clearProperties(
        self
    ):
        super().clearProperties()
        self._p_assignment = ""


    def displayName(
        self
    ):
        ret = self._p_name
        if self.isArgument() and self._p_assignment:
            ret += " ="
        return ret


    def displayView(
        self
    ):
        rt = RichText()
        rt.addHeader(1,"Description")
        parts = self._p_description.split("\n")
        if len(parts) == 2:
            rt.addHeader(3,parts.pop(0))
        rt.addText(self._p_description)
        if self._p_assignment:
            rt.addHeader(1,"Assignment")
            rt.addText(self._p_assignment)
        return rt


    def editDefinitions(
        self
    ):
        edits = super().editDefinitions()
        edits.append(LineEdit("Assignment:","_p_assignment"))
        return edits


    def icon(
        self
    ):
        if self.isArgument() or self.parent() is None:
            return QIcon(":/socref_python/object.svg")
        elif self._p_name.startswith("__"):
            return QIcon(":/socref_python/private_member.svg")
        elif self._p_name.startswith("_"):
            return QIcon(":/socref_python/protected_member.svg")
        else:
            return QIcon(":/socref_python/public_member.svg")


    def isArgument(
        self
    ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this object is an argument of a function or false
               otherwise.
        """
        return self.parent() and self.parent()._TYPE_ == "Function"


    def isVolatileAbove(
        self
    ):
        return True


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "object"
        self._p_assignment = ""
