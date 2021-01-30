"""
Contains the PackageBlock class.
"""
from PySide2.QtGui import QIcon
from socref import block
from socref.Abstract.AbstractBlock import *
from socref.Edit.EditDefinitions import *
from socref.Edit.LineEdit import *
from socref.Edit.TextEdit import *
from socref.Output.RichText import *




@block("Package",root=True)
class PackageBlock(AbstractBlock):
    """
    This is the package block class. It implements the Socrates' Reference
    abstract block class. It represents a python package. It is the root block
    type of a python project.
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
        if self.parent() is None:
            #return ("Package","Module","Class")
            return ("Package",)
        else:
            #return ("Package","Module","Class","Function")
            return ("Package",)


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
        rt.addHeader(1,"Description")
        rt.addText(self._p_description)
        return str(rt)


    def editDefinitions(
        self
    ):
        edits = EditDefinitions()
        edits.append(LineEdit("_p_name","Name:"))
        edits.append(TextEdit("_p_description","Description:"))
        return edits


    def icon(
        self
    ):
        return QIcon(":/python/package.svg")


    def key(
        self
    ):
        """
        Detailed description.
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
        self._p_name = "package"
        self._p_description = "Detailed description."
