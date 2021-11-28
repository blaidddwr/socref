"""
Contains the ObjectBlock class.
"""
from .ModuleBlock import ModuleBlock
from PySide6.QtGui import QIcon
from socref import block
from socref.Edit.LineEdit import LineEdit
from socref.Output.RichText import RichText




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


    def argValues(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : tuple
                 The argument name, assignment, type, and description of this
                 object. The type and description must be separating with a new
                 line in its description field to be populated, otherwise both
                 returns are empty strings.
        """
        type_ = ""
        description = ""
        try:
            (type_,description) = self._p_description.split("\n")
        except:
            pass
        return (self._p_name,self._p_assignment,type_,description)


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
        ret = RichText()
        ret.addHeader("Description",1)
        parts = self._p_description.split("\n")
        if len(parts) == 2:
            ret.addHeader(parts.pop(0),3)
        ret.addText(self._p_description)
        if self._p_assignment:
            ret.addHeader("Assignment",1)
            ret.addText(self._p_assignment)
        return ret


    def editDefinitions(
        self
    ):
        ret = super().editDefinitions()
        ret.append(LineEdit("Assignment:","_p_assignment"))
        return ret


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
        result : bool
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
