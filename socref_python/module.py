"""
Contains the module block definition.
"""
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as util
from socref import abstract
from . import settings
from . import package








@register("Module")
class Block(package.Block):
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
        package.Block.__init__(self)


    ##########################
    # PUBLIC - Basic Methods #
    ##########################


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/python/module.svg")


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ("Object","Function","Class")


    #############################
    # PUBLIC - Property Methods #
    #############################


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        package.Block.set_default_properties(self)
        self._p_name = "module"


    ############################
    # PUBLIC - Parsing Methods #
    ############################


    def file_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        if self._TYPE_ == "Module":
            return self._p_name + ".py"


    def build(self, definition, begin=""):
        """
        Implements the .package.Block interface.

        definition : See interface docs.

        begin : See interface docs.

        return : See interface docs.
        """
        ret = package.Block.build(self,definition,begin)
        ret += self._build_children_(definition,begin)
        #
        # Add the special main function call if this is the special main module of a package.
        #
        if self._p_name == "__main__":
            ret += "\n"*settings.H1LINES + 'if __name__ == "__main__": main()\n'
        return ret
