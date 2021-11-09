"""
Contains the ClassBlock class.
"""
from .NamespaceBlock import NamespaceBlock
from PySide6.QtGui import QIcon
from socref import block
from socref.Edit.TextEdit import TextEdit




@block("Class")
class ClassBlock(NamespaceBlock):
    """
    This is the class block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ class.
    """


    def __init__(
        self
    ):
        super().__init__()
        self._p_parents = ""


    def buildList(
        self
    ):
        return ("Access",)


    def clearProperties(
        self
    ):
        super().clearProperties()
        self._p_parents = ""


    def clsParents(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : list
                 Names of all inherited class parents of this class block.
        """
        return [p for p in self._p_parents.split("\n") if p]


    def displayView(
        self
    ):
        ret = super().displayView()
        parents = self.clsParents()
        if parents:
            ret.addHeader("Parents",1)
            ret.addList(parents)
        return ret


    def editDefinitions(
        self
    ):
        ret = super().editDefinitions()
        ret.append(TextEdit("Parents:","_p_parents"))
        return ret


    def icon(
        self
    ):
        if self.isAbstract():
            return QIcon(":/cpp/abstract_class.svg")
        elif self.isVirtual():
            return QIcon(":/cpp/virtual_class.svg")
        else:
            return QIcon(":/cpp/class.svg")


    def isAbstract(
        self
    ):
        """
        Getter method.
        """
        for child in self:
            if child._TYPE_ == "Access" and child.hasAbstract():
                return True
        return False


    def isVirtual(
        self
    ):
        """
        Getter method.
        """
        for child in self:
            if child._TYPE_ == "Access" and child.hasVirtual():
                return True
        return False


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "class"
        self._p_parents = ""
