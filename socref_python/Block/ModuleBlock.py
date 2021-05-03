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

        Because of the ambiguity of classes being contained within a module of
        the same name, keys must adjust to this fact. A module reader exists for
        the dummy container module for each class, and the returned keys must
        adapt to this. This is why there is a special is module flag given to
        this method, so if this is a class block it knows to not append its name
        twice if this is its dummy module container writer. In all other
        instances of this being the class, nested class, or function writer the
        root class name is correctly appended twice to the returned key.

        Parameters
        ----------
        isMod : bool
                True if the returned key is for a module or false otherwise.

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
