"""
Contains the Class class.
"""
from PySide2 import QtGui as qtg
from socref import edit
from socref import register
from socref_cpp import block as cppblock








@register("Class")
class Class(cppblock.Class):
    """
    This is the class block class. It implements the Socrates' Reference abstract block class. It
    represents a C++/Qt class.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new class block.
        """
        cppblock.Class.__init__(self)
        self._p_qtObject = "0"


    ####################
    # PUBLIC - Methods #
    ####################


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        cppblock.Class.clearProperties(self)
        self._p_qtObject = "0"


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = cppblock.Class.editDefinitions(self)
        ret.append(edit.checkboxEdit("Qt Object","_p_qtObject"))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        if self.isQtObject():
            if self.isAbstract():
                return qtg.QIcon(":/cppqt/abstract_qtobject.svg")
            elif self.isVirtual():
                return qtg.QIcon(":/cppqt/virtual_qtobject.svg")
            else:
                return qtg.QIcon(":/cppqt/qtobject.svg")
        else:
            return cppblock.Class.icon(self)


    def isQtObject(self):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this class is a qt object or false otherwise.
        """
        return bool(int(self._p_qtObject))


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        cppblock.Class.setDefaultProperties(self)
        self._p_qtObject = "0"


    #######################
    # PROTECTED - Methods #
    #######################


    def _buildClassHeader_(self, begin):
        """
        Extends the socref_cpp.block.Class method.

        Parameters
        ----------
        begin : object
                See method docs.

        Returns
        -------
        ret0 : object
               See method docs.
        """
        ret = []
        if self.isQtObject():
            ret.append(begin+"Q_OBJECT")
        return ret+cppblock.Class._buildClassHeader_(self,begin)
