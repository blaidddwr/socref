"""
Contains the FunctionBlock class.
"""
from .ModuleBlock import *
from PySide2.QtGui import QIcon
from socref import block
from socref.Edit.CheckBoxEdit import *
from socref.Edit.HiddenEdit import *
from socref.Edit.TextEdit import *




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
        ,method=True
    ):
        """
        Detailed description.

        Parameters
        ----------
        method : object
                 Detailed description.
        """
        ret = []
        if self.isMethod() and method:
            ret.append(("cls" if self.isClass() else "self","","",""))
        for child in self:
            type_ = ""
            description = ""
            try:
                (type_,description) = child._p_description.split("\n")
            except:
                pass
            ret.append(child._p_name,child._p_assignment,type_,description)
        return ret


    def buildList(
        self
    ):
        #return ("Object",)
        return ()


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
        Detailed description.

        Parameters
        ----------
        flags : object
                Detailed description.
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
        rt = super().displayView()
        rt.addHeader(1,"Returns")
        for (name,type_,text) in self.returns():
            rt.addHeader(3,name+" : "+type_)
            rt.addText(text)
        rt.addHeader(1,"Arguments")
        for (name,assignment,type_,text) in self.arguments(False):
            rt.addHeader(3,name+(" = "+assignment if assignment else "")+" : "+type_)
            rt.addText(text)
        rt.addHeader(1,"Flags")
        rt.addList(self.flags())
        rt.addHeader(1,"Decorators")
        rt.addList(self.decorators(False))
        return rt


    def editDefinitions(
        self
    ):
        edits = super().editDefinitions()
        edits.append(TextEdit("Return:","_p_returnsDescription",True))
        if self.isMethod():
            edits.append(CheckBoxEdit("Static","_p_static"))
            edits.append(CheckBoxEdit("Class","_p_class"))
            edits.append(CheckBoxEdit("Abstract","_p_abstract"))
        else:
            edits.append(HiddenEdit("_p_static","0"))
            edits.append(HiddenEdit("_p_class","0"))
            edits.append(HiddenEdit("_p_abstract","0"))
        edits.append(TextEdit("Descriptors:","_p_decorators"))
        return edits


    def flags(
        self
    ):
        """
        Detailed description.
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
        Detailed description.
        """
        return len(self) > 0 or self.isMethod()


    def icon(
        self
    ):
        if not self.isMethod():
            return QIcon(":/python/function.svg")
        elif self._p_name.startswith("__") and self._p_name.endswith("__"):
            return QIcon(":/python/operator.svg")
        elif self._p_name.startswith("__"):
            if self.isStatic():
                return QIcon(":/python/static_private_method.svg")
            elif self.isClass():
                return QIcon(":/python/class_private_method.svg")
            elif self.isAbstract():
                return QIcon(":/python/abstract_private_method.svg")
            else:
                return QIcon(":/python/private_method.svg")
        elif self._p_name.startswith("_"):
            if self.isStatic():
                return QIcon(":/python/static_protected_method.svg")
            elif self.isClass():
                return QIcon(":/python/class_protected_method.svg")
            elif self.isAbstract():
                return QIcon(":/python/abstract_protected_method.svg")
            else:
                return QIcon(":/python/protected_method.svg")
        else:
            if self.isStatic():
                return QIcon(":/python/static_public_method.svg")
            elif self.isClass():
                return QIcon(":/python/class_public_method.svg")
            elif self.isAbstract():
                return QIcon(":/python/abstract_public_method.svg")
            else:
                return QIcon(":/python/public_method.svg")


    def isAbstract(
        self
    ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
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
        ret0 : bool
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
        ret0 : bool
               True if this function is a method of a class or false otherwise.
        """
        return self.parent() and self.parent()._TYPE_ == "Class"


    def isStatic(
        self
    ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this function is static or false otherwise.
        """
        return bool(int(self._p_static))


    def returns(
        self
    ):
        """
        Detailed description.
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
