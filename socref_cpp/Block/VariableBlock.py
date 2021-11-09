"""
Contains the VariableBlock class.
"""
from .NamespaceBlock import NamespaceBlock
from PySide6.QtGui import QIcon
from socref import block
from socref.Edit.LineEdit import LineEdit
from socref.Edit.CheckboxEdit import CheckboxEdit
from enum import (
    IntEnum
    auto
)




@block("Variable")
class VariableBlock(NamespaceBlock):
    """
    This is the variable block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ variable. The variable block's
    type field has a special @ character which is a placeholder for where the
    name should be inserted within the type field.
    """
    FLAGS = {
        "X": ("X","constexpr","Constant Expression")
        ,"L": ("L","thread_local","Thread Local")
    }


    class FlagOutput(IntEnum):
        """
        This enumerates all possible output types for a variable block's flags.
        """
        Compact = 0
        Code = 1
        Full = 2


    def __init__(
        self
    ):
        super().__init__()
        self._p_type = ""
        self._p_assignment = ""
        self._p_constexpr = "0"
        self._p_thread_local = "0"


    def buildList(
        self
    ):
        return ()


    def clearProperties(
        self
    ):
        super().clearProperties()
        self._p_type = ""
        self._p_assignment = ""
        self._p_constexpr = "0"
        self._p_thread_local = "0"


    def displayName(
        self
    ):
        flags = "".join(self.flags(FlagOutput.Compact))
        return "".join(
            (self._p_name," *" if self._p_assignment else ""," ["+flags+"]" if flags else "")
        )


    def displayView(
        self
    ):
        ret = super().displayView()
        ret.addHeader("Type",1)
        ret.addText(self._p_type)
        if self._p_assignment:
            ret.addHeader("Assignment",1)
            ret.addText(self._p_assignment)
        flags = self.flags(FlagOutput.Full)
        if flags:
            ret.addHeader("Flags",1)
            ret.addList(flags)
        return ret


    def editDefinitions(
        self
    ):
        ret = super().editDefinitions()
        ret.append(LineEdit("Type:","_p_type"))
        ret.append(LineEdit("Assignment:","_p_assignment"))
        if not self.isArgument():
            ret.append(CheckboxEdit("Constant Expression","_p_constexpr"))
            ret.append(CheckboxEdit("Thread Local","_p_thread_local"))
        else:
            ret.append(HiddenEdit("_p_constexpr","0"))
            ret.append(HiddenEdit("_p_thread_local","0"))
        return ret


    def flags(
        self
        ,output
    ):
        """
        Detailed description.
        """
        ret = []
        if self.isConstExpr():
            ret.append("X")
        if self.isThreadLocal():
            ret.append("L")
        return [self.FLAGS[f][output] for f in ret]


    def icon(
        self
    ):
        if self.isStatic():
            return QIcon(":/cpp/static_variable.svg")
        else:
            return QIcon(":/cpp/variable.svg")


    def isArgument(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this variable is an argument of a function or false
                 otherwise.
        """
        return self.parent()._TYPE_ == "Function"


    def isConstExpr(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this variable is a constant expression or false
                 otherwise.
        """
        return bool(int(self._p_constexpr))


    def isThreadLocal(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this variable is thread local or false otherwise.
        """
        return bool(int(self._p_thread_local))


    def isVolatileAbove(
        self
    ):
        return self.isArgument()


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "variable"
        self._p_type = "int @"
        self._p_constexpr = "0"
        self._p_thread_local = "0"


FlagOutput = VariableBlock.FlagOutput
