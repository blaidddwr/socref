"""
Contains the NamespaceBlock class.
"""
from ..Settings import Settings
from PySide6.QtGui import QIcon
from socref import block
from socref.Base.BlockBase import BlockBase
from socref.Edit.LineEdit import LineEdit
from socref.Edit.TextEdit import TextEdit
from socref.Output.RichText import RichText




@block("Namespace",root=True)
class NamespaceBlock(BlockBase):
    """
    This is the name space block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ name space.
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
        return ("Namespace","Class","Enumeration","Function","Union")


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
                 True if this name space block has one or more functions or
                 false otherwise.
        """
        for child in self:
            if child._TYPE_ == "Function":
                return True
        return False


    def icon(
        self
    ):
        return QIcon(":/socref_cpp/namespace.svg")


    def isRoot(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this namespace is the global root or false otherwise.
        """
        return bool(self.parent() == None)


    def key(
        self
    ):
        """
        Returns a unique key used to obtain the correct reader or writer of this
        block. The returned unique key can also be used as the file name for
        header and source files because it only contains the names of all scoped
        namespaces and possible class.

        Returns
        -------
        result : string
                 The unique key.
        """
        if self.isRoot():
            return Settings.GLOBAL_NS_KEY
        else:
            return "".join(self.scope()+[self._p_name])


    def scope(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : list
                 Names of name spaces where this block resides, starting from
                 the global name space and ending in the last scoped. This
                 includes any class name spaces.
        """
        names = []
        b = self.parent()
        while b is not None and b.parent() is not None:
            if b._TYPE_ == "Namespace" or b._TYPE_ == "Class":
                names.append(b._p_name)
            b = b.parent()
        names.reverse()
        return names


    def setDefaultProperties(
        self
    ):
        self._p_name = "namespace"
        self._p_description = "Detailed description."
