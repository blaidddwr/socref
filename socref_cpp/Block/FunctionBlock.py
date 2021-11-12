"""
Contains the FunctionBlock class.
"""
from .NamespaceBlock import NamespaceBlock
from PySide6.QtGui import QIcon
from socref import block
from socref.Edit.CheckBoxEdit import CheckBoxEdit
from socref.Edit.ComboEdit import ComboEdit
from socref.Edit.HiddenEdit import HiddenEdit
from socref.Edit.LineEdit import LineEdit
from socref.Edit.TextEdit import TextEdit
from enum import IntEnum




@block("Function")
class FunctionBlock(NamespaceBlock):
    """
    This is the function block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ function or method. The special
    function names '^' and '~^' are reserved for class constructors and
    destructors, respectively.
    """
    ACCESS = (
        ("Public",None)
        ,("Protected",None)
        ,("Private",None)
    )
    ACCESS_CHAR = {
        "Public": "* "
        ,"Protected": "^ "
        ,"Private": "- "
    }
    FLAGS = {
        "D": ("D","= default","Default")
        ,"R": ("R","= delete","Deleted")
        ,"E": ("E","explicit","Explicit")
        ,"C": ("C","const","Constant")
        ,"S": ("S","static","Static")
        ,"N": ("N","noexcept","No Exceptions")
        ,"V": ("V","virtual","Virtual")
        ,"O": ("O","override","Override")
        ,"F": ("F","final","Final")
        ,"A": ("A","= 0","Abstract")
    }


    class FlagOutput(IntEnum):
        """
        This enumerates all possible output types for a function block's flags.
        """
        Compact = 0
        Code = 1
        Full = 2


    def __init__(
        self
    ):
        super().__init__()
        self._p_access = "Public"
        self._p_returnType = ""
        self._p_returnDescription = ""
        self._p_default = "0"
        self._p_deleted = "0"
        self._p_explicit = "0"
        self._p_virtual = "0"
        self._p_const = "0"
        self._p_static = "0"
        self._p_noexcept = "0"
        self._p_override = "0"
        self._p_final = "0"
        self._p_abstract = "0"


    def arguments(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : list
                 A list of argument tuples for this function. Each tuple
                 contains the name, assignment, type, and description in that
                 order. The assignment and description can be empty.
        """
        ret = []
        for child in self:
            ret.append((child._p_name,child._p_assignment,child._p_type,child._p_description))
        return ret


    def buildList(
        self
    ):
        return ("Variable",)


    def clearProperties(
        self
    ):
        super().clearProperties()
        self._p_access = "Public"
        self._p_returnType = ""
        self._p_returnDescription = ""
        self._p_default = "0"
        self._p_deleted = "0"
        self._p_explicit = "0"
        self._p_virtual = "0"
        self._p_const = "0"
        self._p_static = "0"
        self._p_noexcept = "0"
        self._p_override = "0"
        self._p_final = "0"
        self._p_abstract = "0"


    def displayName(
        self
    ):
        (before,after) = self.flags(FlagOutput.Compact)
        flags = "".join(before+after)
        return "".join(
            (
                self.ACCESS_CHAR[self._p_access] if self.isMethod() else ""
                ,self.name()
                ," ..." if self.hasReturn() else ""
                ," ["+flags+"]" if flags else "")
        )


    def displayView(
        self
    ):
        ret = super().displayView()
        arguments = self.arguments()
        if arguments:
            ret.addHeader("Arguments",1)
            for (name,assignment,type_,text) in arguments:
                ret.addBox(name+(" = "+assignment if assignment else "")+" : "+type_,text)
        if self.hasReturn():
            ret.addHeader("Return",1)
            ret.addBox(self._p_returnType,self._p_returnDescription)
        (before,after) = self.flags(FlagOutput.Full)
        flags = before+after
        if flags:
            ret.addHeader("Flags",1)
            ret.addList(flags)
        return ret


    def editDefinitions(
        self
    ):
        ret = super().editDefinitions()
        if self.isMethod():
            ret.append(ComboEdit("Access:","_p_access",self.ACCESS))
        else:
            ret.append(HiddenEdit("_p_access","Public"))
        ret.append(LineEdit("Return Type:","_p_returnType"))
        ret.append(TextEdit("Return Description:","_p_returnDescription",True))
        ret.append(CheckBoxEdit("No Exceptions","_p_noexcept"))
        if self.isMethod():
            ret.append(CheckBoxEdit("Default","_p_default"))
            ret.append(CheckBoxEdit("Deleted","_p_deleted"))
            ret.append(CheckBoxEdit("Explicit","_p_explicit"))
            ret.append(CheckBoxEdit("Constant","_p_const"))
            ret.append(CheckBoxEdit("Static","_p_static"))
            ret.append(CheckBoxEdit("Virtual","_p_virtual"))
            ret.append(CheckBoxEdit("Override","_p_override"))
            ret.append(CheckBoxEdit("Final","_p_final"))
            ret.append(CheckBoxEdit("Abstract","_p_abstract"))
        else:
            ret.append(HiddenEdit("_p_default","0"))
            ret.append(HiddenEdit("_p_deleted","0"))
            ret.append(HiddenEdit("_p_explicit","0"))
            ret.append(HiddenEdit("_p_const","0"))
            ret.append(HiddenEdit("_p_static","0"))
            ret.append(HiddenEdit("_p_virtual","0"))
            ret.append(HiddenEdit("_p_override","0"))
            ret.append(HiddenEdit("_p_final","0"))
            ret.append(HiddenEdit("_p_abstract","0"))
        return ret


    def flags(
        self
        ,output
    ):
        """
        Returns before and after lists of this function block's enabled flags
        with the given output type. The before list are flags that go before the
        function name in code and the after list are flags that go after.

        Parameters
        ----------
        output : FlagOutput
                 The output type.

        Returns
        -------
        before : list
                 The before list.
        after : list
                The after list.
        """
        before = []
        after = []
        if self.isDefault():
            after.append("D")
        if self.isDeleted():
            after.append("R")
        if self.isExplicit():
            before.append("E")
        if self.isConstant():
            after.append("C")
        if self.isStatic():
            before.append("S")
        if self.isNoExcept():
            after.append("N")
        if self.isVirtual():
            before.append("V")
        if self.isOverride():
            after.append("O")
        if self.isFinal():
            after.append("F")
        if self.isAbstract():
            after.append("A")
        return ([self.FLAGS[f][output] for f in before],[self.FLAGS[f][output] for f in after])


    def hasReturn(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function has a return value or false if it is
                 void.
        """
        return self._p_returnType != "void"


    def icon(
        self
    ):
        a = self._p_access.lower()
        if self.isConstructor():
            return QIcon(":/socref_cpp/"+a+"_constructor.svg")
        elif self.isDestructor():
            if self.isAbstract():
                return QIcon(":/socref_cpp/abstract_"+a+"_destructor.svg")
            elif self.isVirtual():
                return QIcon(":/socref_cpp/virtual_"+a+"_destructor.svg")
            else:
                return QIcon(":/socref_cpp/"+a+"_destructor.svg")
        elif self.isOperator():
            if self.isAbstract():
                return QIcon(":/socref_cpp/abstract_"+a+"_operator.svg")
            elif self.isVirtual():
                return QIcon(":/socref_cpp/virtual_"+a+"_operator.svg")
            else:
                return QIcon(":/socref_cpp/"+a+"_operator.svg")
        else:
            if self.isAbstract():
                return QIcon(":/socref_cpp/abstract_"+a+"_function.svg")
            elif self.isVirtual():
                return QIcon(":/socref_cpp/virtual_"+a+"_function.svg")
            elif self.isStatic():
                return QIcon(":/socref_cpp/static_"+a+"_function.svg")
            else:
                return QIcon(":/socref_cpp/"+a+"_function.svg")


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


    def isConstant(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is constant or false otherwise.
        """
        return bool(int(self._p_const))


    def isConstructor(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is a constructor or false otherwise.
        """
        return self._p_name == "^"


    def isDefault(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is the default or false otherwise.
        """
        return bool(int(self._p_default))


    def isDeleted(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is deleted or false otherwise.
        """
        return bool(int(self._p_deleted))


    def isDestructor(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is a destructor or false otherwise.
        """
        return self._p_name == "~^"


    def isExplicit(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is an explicit constructor or false
                 otherwise.
        """
        return bool(int(self._p_explicit))


    def isFinal(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is the final overriding virtual interface
                 or false otherwise.
        """
        return bool(int(self._p_final))


    def isMethod(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is a method or false otherwise.
        """
        return self.parent() and self.parent()._TYPE_ == "Class"


    def isNoExcept(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function does not throw exceptions or false
                 otherwise.
        """
        return bool(int(self._p_noexcept))


    def isOperator(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is an operator or false otherwise.
        """
        return self._p_name.startswith("operator")


    def isOverride(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is overriding a virtual interface or
                 false otherwise.
        """
        return bool(int(self._p_override))


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


    def isVirtual(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is virtual or false otherwise.
        """
        return bool(int(self._p_virtual))


    def isVolatileAbove(
        self
    ):
        return self.isMethod()


    def name(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : string
                 The name of this function, taking into account if it is a
                 constructor or destructor.
        """
        if self._p_name == "^":
            return self.parent()._p_name if self.isMethod() else "INVALID"
        elif self._p_name == "~^":
            return "~"+self.parent()._p_name if self.isMethod() else "~INVALID"
        else:
            return self._p_name


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_type = "Public"
        self._p_name = "function"
        self._p_returnType = "void"
        self._p_returnDescription = ""
        self._p_default = "0"
        self._p_deleted = "0"
        self._p_explicit = "0"
        self._p_virtual = "0"
        self._p_const = "0"
        self._p_static = "0"
        self._p_noexcept = "0"
        self._p_override = "0"
        self._p_final = "0"
        self._p_abstract = "0"


FlagOutput = FunctionBlock.FlagOutput
