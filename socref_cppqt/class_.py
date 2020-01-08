"""
Contains the class block class.
"""
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from socref_cpp import class_ as cppclass








@register("Class")
class Class(cppclass.Class):
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
        cppclass.Class.__init__(self)
        self._p_qtObject = "0"


    ####################
    # PUBLIC - Methods #
    ####################


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        cppclass.Class.clearProperties(self)
        self._p_qtObject = "0"


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = cppclass.Class.editDefinitions(self)
        ret.append(ut.checkboxEdit("Qt Object","_p_qtObject"))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        if self.isQtObject():
            if self.isAbstract():
                return qtg.QIcon(":/cppqt/abstract_qtobject.svg")
            elif self.isVirtual():
                return qtg.QIcon(":/cppqt/virtual_qtobject.svg")
            else:
                return qtg.QIcon(":/cppqt/qtobject.svg")
        else:
            return cppclass.Class.icon(self)


    def isQtObject(self):
        """
        Getter method.

        return : True if this class is a qt object or false otherwise.
        """
        return bool(int(self._p_qtObject))


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        cppclass.Class.setDefaultProperties(self)
        self._p_qtObject = "0"


    #######################
    # PROTECTED - Methods #
    #######################


    def _buildClassHeader_(self, begin):
        """
        Extends the socref_cpp.class_.Class method.

        begin : See method docs.

        return : See method docs.
        """
        ret = []
        if self.isQtObject():
            ret.append(begin+"Q_OBJECT")
        return ret+cppclass.Class._buildClassHeader_(self,begin)
