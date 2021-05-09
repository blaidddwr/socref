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
    abstract block class. It represents a python module. This class acts as a
    base block class inherited by all other block implementations.
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
        return QIcon(":/socref_python/module.svg")


    def key(
        self
        ,isMod=False
    ):
        """
        Getter method.

        Because a module reader exists for the dummy container module for each
        class, keys must adjust to this fact. The given module flag must be true
        if a module writer is calling this method or false otherwise. In this
        way the class name is correctly used only once for the dummy module
        writer to find its dummy module reader. In all other instances of this
        being the class, nested class, or function writer the root class name is
        correctly appended twice in order for the writer to find its reader.

        Parameters
        ----------
        isMod : bool
                True if the returned key is for a module writer or false
                otherwise.

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
