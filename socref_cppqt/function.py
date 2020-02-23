"""
Contains the function block class.
"""
from PySide2 import QtGui as qtg
from socref import register
from socref_cpp import function as cppfunction








@register("Function")
class Function(cppfunction.Function):
    """
    This is the function block class. It implements the Socrates' Reference abstract block class. It
    represents a C++/Qt function.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new function block.
        """
        cppfunction.Function.__init__(self)


    ####################
    # PUBLIC - Methods #
    ####################


    def buildDefinition(self, definitions, scope, template, header):
        """
        Implements the socref_cpp.namespace.Base interface.

        Parameters
        ----------
        definitions : object
                      See interface docs.
        scope : object
                See interface docs.
        template : object
                   See interface docs.
        header : object
                 See interface docs.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        if self.isMethod() and self.parent().isSignals():
            return ([],[])
        else:
            return cppfunction.Function.buildDefinition(self,definitions,scope,template,header)


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        if self.isMethod():
            if self.parent().isSignals():
                return qtg.QIcon(":/cppqt/signal.svg")
            elif self.parent().isSlots():
                if self.isAbstract():
                    return qtg.QIcon(":/cppqt/abstract_slot.svg")
                elif self.isVirtual():
                    return qtg.QIcon(":/cppqt/virtual_slot.svg")
                else:
                    return qtg.QIcon(":/cppqt/slot.svg")
            else:
                return cppfunction.Function.icon(self)
        else:
            return cppfunction.Function.icon(self)
