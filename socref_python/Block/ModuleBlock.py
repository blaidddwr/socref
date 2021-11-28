"""
Contains the ModuleBlock class.
"""
from PySide6.QtGui import QIcon
from socref import block
from socref.Base.BlockBase import BlockBase
from socref.Edit.LineEdit import LineEdit
from socref.Edit.TextEdit import TextEdit
from socref.Output.RichText import RichText




@block("Module")
class ModuleBlock(BlockBase):
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
        return QIcon(":/socref_python/module.svg")


    def key(
        self
        ,module=False
    ):
        """
        Returns a unique key used to obtain the correct reader or writer of this
        block with the given optional module flag. Because a module reader
        exists for the dummy container module for each class, keys must adjust
        to this fact. The given module flag must be true if a module writer is
        calling this method or false otherwise. In this way the class name is
        correctly used only once for the dummy module writer to find its dummy
        module reader. In all other instances of this being the class, nested
        class, or function writer the root class name is correctly appended
        twice in order for the writer to find its reader.

        Parameters
        ----------
        module : bool
                 True if the returned key is for a module writer or false
                 otherwise.

        Returns
        -------
        result : string
                 The unique key.
        """
        names = []
        b = self
        while b is not None:
            if b._p_name:
                names.append(b._p_name)
                if b._TYPE_ == "Class" and not b.isInfile() and not module:
                    names.append(b._p_name)
            b = b.parent()
        names.reverse()
        return ".".join(names)


    def setDefaultProperties(
        self
    ):
        self._p_name = "module"
        self._p_description = "Detailed description."
