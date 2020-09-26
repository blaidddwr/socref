"""
Contains the Access class.
"""
from PySide2 import QtGui as qtg
from socref import public as scr
from socref_cpp import block as cppblock








@scr.register("Access")
class Access(cppblock.Access):
    """
    This is the access block class. It implements the Socrates' Reference
    abstract block class. It represents an access declaration for C++/Qt class.
    """


    def icon(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        if self._p_type == "Signals":
            return qtg.QIcon(":/cppqt/signals.svg")
        elif self._p_type == "Public Slots":
            return qtg.QIcon(":/cppqt/public_slots.svg")
        elif self._p_type == "Protected Slots":
            return qtg.QIcon(":/cppqt/protected_slots.svg")
        elif self._p_type == "Private Slots":
            return qtg.QIcon(":/cppqt/private_slots.svg")
        else:
            return cppblock.Access.icon(self)


    def isSignals(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this access is a signals type or false otherwise.
        """
        return self._p_type == "Signals"


    def isSlots(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this access is a public, protected, or private slots
               type. False is returned otherwise.
        """
        return self._p_type.endswith("Slots")


    def _addComboSelects_(
        self
        ,combo
        ):
        """
        Extends the socref_cpp.block.Access method.

        Parameters
        ----------
        combo : object
                See method docs.
        """
        cppblock.Access._addComboSelects_(self,combo)
        scr.addComboSelect(combo,"Signals",icon=qtg.QIcon(":/cppqt/signals.svg"))
        scr.addComboSelect(combo,"Public Slots",icon=qtg.QIcon(":/cppqt/public_slots.svg"))
        scr.addComboSelect(combo,"Protected Slots",icon=qtg.QIcon(":/cppqt/protected_slots.svg"))
        scr.addComboSelect(combo,"Private Slots",icon=qtg.QIcon(":/cppqt/private_slots.svg"))
