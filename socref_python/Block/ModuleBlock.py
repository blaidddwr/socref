"""
Contains the ModuleBlock class.
"""
from PySide2.QtGui import QIcon
from socref import block
from socref.Abstract.AbstractBlock import *
from socref.Edit.LineEdit import *
from socref.Edit.TextEdit import *
from socref.Output.RichText import *




@block("Module")
class ModuleBlock(AbstractBlock):
    """
    This is the module block class. It implements the Socrates' Reference
    abstract block class. It represents a python module.
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
        return ("Function","Class")


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
        return rt


    def editDefinitions(
        self
    ):
        return [LineEdit("Name:","_p_name"),TextEdit("Description:","_p_description",True)]


    def icon(
        self
    ):
        return QIcon(":/socref_python/module.svg")


    def key(
        self
        ,isMod=False
    ):
        """
        Detailed description.

        Parameters
        ----------
        isMod : 
        """
        names = []
        b = self
        while b is not None:
            if b._p_name:
                names.append(b._p_name)
                if b._TYPE_ == "Class" and not b.isInfile() and not isMod:
                    names.append(b._p_name)
            b = b.parent()
        names.reverse()
        return ".".join(names)


    def setDefaultProperties(
        self
    ):
        self._p_name = "module"
        self._p_description = "Detailed description."
