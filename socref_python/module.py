"""
Contains the module block definition.
"""
from PySide2 import QtGui as qtg
from socref import register
from socref import abstract
from . import settings
from . import package








@register("Module")
class Module(package.Package):
    """
    This is the module block class. It implements the Socrates' Reference abstract block class. It
    represents a python module.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new module block.
        """
        package.Package.__init__(self)


    ############################
    # PUBLIC - Parsing Methods #
    ############################


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/python/module.svg")


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Object","Function","Class")


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        package.Package.setDefaultProperties(self)
        self._p_name = "module"


    def file_name(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        if self._TYPE_ == "Module":
            return self._p_name + ".py"


    def build(self, definition, begin=""):
        """
        Implements the .package.Package interface.

        definition : See interface docs.

        begin : See interface docs.

        return : See interface docs.
        """
        ret = package.Package.build(self,definition,begin)
        ret += self._build_children_(definition,begin)
        #
        # Add the special main function call if this is the special main module of a package.
        #
        if self._p_name == "__main__":
            ret += "\n"*settings.H1LINES + 'if __name__ == "__main__": main()\n'
        return ret
