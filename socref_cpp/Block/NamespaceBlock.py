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


    def key(
        self
        ,isFile=False
    ):
        """
        Returns a unique key used to obtain the correct reader or writer of this
        block with the given optional file flag. Because C++ has no association
        between its scoping and file structure, a distinction must be made
        between a header or source file and the name space or class it contains.
        The given file flag must be true if a file reader or writer is calling
        this method or false otherwise.

        Parameters
        ----------
        isFile : bool
                 The file flag.

        Returns
        -------
        result : string
                 The unique key.
        """
        div = "/" if isFile else "::"
        scope = self.scope()
        if isFile and self._TYPE_ == "Namespace":
            scope += [self._p_name,"__init__"] if self.parent() else ["__init__"]
        elif self._TYPE_ == "Function":
            scope.append(self.name())
        else:
            scope.append(self._p_name)
        return div.join(scope)


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
