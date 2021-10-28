"""
Contains the FunctionBlock class.
"""
from .ModuleBlock import ModuleBlock
from PySide6.QtGui import QIcon
from socref import block
from socref.Edit.CheckBoxEdit import CheckBoxEdit
from socref.Edit.HiddenEdit import HiddenEdit
from socref.Edit.TextEdit import TextEdit




@block("Function")
class FunctionBlock(ModuleBlock):
    """
    This is the function block class. It implements the Socrates' Reference
    abstract block class. It represents a python function.
    """


    def __init__(
        self
    ):
        super().__init__()
        self._p_returnsDescription = ""
        self._p_static = "0"
        self._p_class = "0"
        self._p_abstract = "0"
        self._p_decorators = ""


    def arguments(
        self
        ,special=True
    ):
        """
        Returns a list of argument tuples for this function with the given
        optional special flag. Each tuple contains the name, assignment, type,
        and description in that order. The assignment, type, and description can
        be empty. The given special flag is true to include the special cls or
        self arguments to the returned arguments if this function is a regular
        or class method, else false to exclude them.

        Parameters
        ----------
        special : bool
                  The special flag.

        Returns
        -------
        result : list
                 The argument tuples.
        """
        ret = []
        if self.isMethod() and not self.isStatic() and special:
            ret.append((("cls" if self.isClass() else "self"),"","",""))
        for child in self:
            ret.append(child.argValues())
        return ret


    def buildList(
        self
    ):
        return ("Object",)


    def clearProperties(
        self
    ):
        super().clearProperties()
        self._p_returnsDescription = ""
        self._p_static = "0"
        self._p_class = "0"
        self._p_abstract = "0"
        self._p_decorators = ""


    def decorators(
        self
        ,flags=True
    ):
        """
        Getter method.

        Parameters
        ----------
        flags : 

        Returns
        -------
        result : list
                 Decorator lines of this function. Each line includes the
                 beginning special @ character.
        """
        ret = []
        if flags:
            if self.isStatic():
                ret.append("@staticmethod")
            if self.isClass():
                ret.append("@classmethod")
            if self.isAbstract():
                ret.append("@abstractmethod")
        ret += ["@"+d for d in self._p_decorators.split("\n") if d]
        return ret


    def displayName(
        self
    ):
        ret = ""
        ret += "%s(%i)" % (self._p_name,len(self))
        if self._p_decorators:
            ret += " @"
        if self._p_returnsDescription:
            ret += " ..."
        return ret


    def displayView(
        self
    ):
        ret = super().displayView()
        arguments = self.arguments(False)
        if arguments:
            ret.addHeader("Arguments",1)
            for (name,assignment,type_,text) in arguments:
                ret.addBox(name+(" = "+assignment if assignment else "")+" : "+type_,text)
        returns = self.returns()
        if returns:
            ret.addHeader("Returns",1)
            for (name,type_,text) in returns:
                ret.addBox(name+" : "+type_,text)
        flags = self.flags()
        if flags:
            ret.addHeader("Flags",1)
            ret.addList(self.flags())
        decorators = self.decorators(False)
        if decorators:
            ret.addHeader("Decorators",1)
            ret.addList(decorators)
        return ret


    def editDefinitions(
        self
    ):
        ret = super().editDefinitions()
        ret.append(TextEdit("Return:","_p_returnsDescription",True))
        if self.isMethod():
            ret.append(CheckBoxEdit("Static","_p_static"))
            ret.append(CheckBoxEdit("Class","_p_class"))
            ret.append(CheckBoxEdit("Abstract","_p_abstract"))
        else:
            ret.append(HiddenEdit("_p_static","0"))
            ret.append(HiddenEdit("_p_class","0"))
            ret.append(HiddenEdit("_p_abstract","0"))
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
                 function.
        """
        ret = []
        if self.isStatic():
            ret.append("Static")
        if self.isClass():
            ret.append("Class")
        if self.isAbstract():
            ret.append("Abstract")
        return ret


    def hasArguments(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function has any arguments or false otherwise.
        """
        return self or (self.isMethod() and not self.isStatic())


    def icon(
        self
    ):
        if not self.isMethod():
            return QIcon(":/socref_python/function.svg")
        elif self._p_name.startswith("__") and self._p_name.endswith("__"):
            return QIcon(":/socref_python/operator.svg")
        elif self._p_name.startswith("__"):
            if self.isStatic():
                return QIcon(":/socref_python/static_private_method.svg")
            elif self.isClass():
                return QIcon(":/socref_python/class_private_method.svg")
            elif self.isAbstract():
                return QIcon(":/socref_python/abstract_private_method.svg")
            else:
                return QIcon(":/socref_python/private_method.svg")
        elif self._p_name.startswith("_"):
            if self.isStatic():
                return QIcon(":/socref_python/static_protected_method.svg")
            elif self.isClass():
                return QIcon(":/socref_python/class_protected_method.svg")
            elif self.isAbstract():
                return QIcon(":/socref_python/abstract_protected_method.svg")
            else:
                return QIcon(":/socref_python/protected_method.svg")
        else:
            if self.isStatic():
                return QIcon(":/socref_python/static_public_method.svg")
            elif self.isClass():
                return QIcon(":/socref_python/class_public_method.svg")
            elif self.isAbstract():
                return QIcon(":/socref_python/abstract_public_method.svg")
            else:
                return QIcon(":/socref_python/public_method.svg")


    def isAbstract(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is abstract or false otherwise.
        """
        return bool(int(self._p_abstract))


    def isClass(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is a class method or false otherwise.
        """
        return bool(int(self._p_class))


    def isMethod(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is a method of a class or false
                 otherwise.
        """
        return self.parent() and self.parent()._TYPE_ == "Class"


    def isStatic(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is static or false otherwise.
        """
        return bool(int(self._p_static))


    def isVolatileAbove(
        self
    ):
        return True


    def returns(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : list
                 Tuples of return values of this function. Each tuple contains
                 the name, type and description in that order.
        """
        ret = []
        for r in self._p_returnsDescription.split("\n\n"):
            if r:
                w = r.split("\n")
                if len(w) == 3:
                    ret.append(tuple(w))
        return ret


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "function"
        self._p_returnsDescription = ""
        self._p_static = "0"
        self._p_class = "0"
        self._p_abstract = "0"
        self._p_decorators = ""
