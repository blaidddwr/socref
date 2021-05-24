"""
Contains the ClassBlock class.
"""
from .ModuleBlock import *
from PySide6.QtGui import QIcon
from socref import block
from socref.Edit.CheckBoxEdit import *
from socref.Edit.HiddenEdit import *
from socref.Edit.TextEdit import *




@block("Class")
class ClassBlock(ModuleBlock):
    """
    This is the class block class. It implements the Socrates' Reference
    abstract block class. It represents a python class.
    """


    def __init__(
        self
    ):
        super().__init__()
        self._p_parents = ""
        self._p_infile = "0"
        self._p_decorators = ""


    def buildList(
        self
    ):
        return ("Function","Object","Class")


    def clearProperties(
        self
    ):
        super().clearProperties()
        self._p_parents = ""
        self._p_infile = "0"
        self._p_decorators = ""


    def clsParents(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : list
                 Names of all inherited class parents of this class.
        """
        return [p for p in self._p_parents.split("\n") if p]


    def decorators(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : list
                 Decorator lines, each one including the beginning special at
                 character, of this class.
        """
        return ["@"+d for d in self._p_decorators.split("\n") if d]


    def displayName(
        self
    ):
        ret = self._p_name
        if self._p_decorators:
            ret += " @"
        return ret


    def displayView(
        self
    ):
        ret = super().displayView()
        parents = self.clsParents()
        if parents:
            ret.addHeader("Parents",1)
            ret.addList(parents)
        decorators = self.decorators()
        if decorators:
            ret.addHeader("Decorators",1)
            ret.addList(decorators)
        flags = self.flags()
        if flags:
            ret.addHeader("Flags",1)
            ret.addList(flags)
        return ret


    def editDefinitions(
        self
    ):
        ret = super().editDefinitions()
        ret.append(TextEdit("Parents:","_p_parents"))
        if self.parent()._TYPE_ == "Class":
            ret.append(HiddenEdit("Infile","0"))
        else:
            ret.append(CheckBoxEdit("Infile","_p_infile"))
        ret.append(TextEdit("Decorators:","_p_decorators"))
        return ret


    def flags(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : list
                 Human readable, capitalized, and enabled flag names of this
                 class.
        """
        return ["Infine"] if self.isInfile() else []


    def icon(
        self
    ):
        if self.isAbstract():
            return QIcon(":/socref_python/abstract_class.svg")
        else:
            return QIcon(":/socref_python/class.svg")


    def isAbstract(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this class contains any abstract functions or false
                 otherwise.
        """
        for child in self:
            if child._TYPE_ == "Function" and child.isAbstract():
                return True
        return False


    def isInfile(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this class is in file or false otherwise. In file means
                 its definition is included in the module it is part of,
                 otherwise it is defined in its own special module named
                 identically to this class.
        """
        return bool(int(self._p_infile)) or (self.parent() and self.parent()._TYPE_ == "Class")


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "class"
        self._p_parents = ""
        self._p_infile = "0"
        self._p_decorators = ""
