"""
Contains the FunctionBlock class.
"""
from PySide6.QtGui import QIcon
from socref import block
from socref_cpp.Block.FunctionBlock import FunctionBlock as CppFunctionBlock




@block("Function")
class FunctionBlock(CppFunctionBlock):
    """
    This is the function block class. It implements the Socrates' Reference
    abstract block class. It represents a C++/Qt function or method.
    """
    QT_ACCESS = (
        ("Signals",QIcon(":/socref_cppqt/signal.svg"))
        ,("Public Slots",QIcon(":/socref_cppqt/public_slot.svg"))
        ,("Protected Slots",QIcon(":/socref_cppqt/protected_slot.svg"))
        ,("Private Slots",QIcon(":/socref_cppqt/private_slot.svg"))
    )
    QT_ACCESS_CHAR = {
        "Signals": "!"
        ,"Public Slots": "@"
        ,"Protected Slots": "%"
        ,"Private Slots": "="
    }


    def hasDefinition(
        self
    ):
        return (
            super().hasDefinition()
            and not self.isSignal()
        )


    def icon(
        self
    ):
        if self.isSignal():
            return QIcon(":/socref_cppqt/signal.svg")
        elif self.isSlot():
            a = self._p_access.lower().replace(" ","_")[:-1]
            if self.isAbstract():
                return QIcon(":/socref_cppqt/abstract_"+a+".svg")
            elif self.isVirtual():
                return QIcon(":/socref_cppqt/virtual_"+a+".svg")
            else:
                return QIcon(":/socref_cppqt/"+a+".svg")
        else:
            return super().icon()


    def isSignal(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is a signal or false otherwise.
        """
        return self._p_access == "Signals"


    def isSlot(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this function is a slot or false otherwise.
        """
        return self._p_access.endswith(" Slots")


    def _access_(
        self
    ):
        return super()._access_()+self.QT_ACCESS


    def _accessChar_(
        self
        ,key
    ):
        if key in self.QT_ACCESS_CHAR:
            return self.QT_ACCESS_CHAR[key]
        else:
            return super()._accessChar_(key)
