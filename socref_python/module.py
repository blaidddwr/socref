"""
Contains the module block definition.
"""
from PySide2 import QtGui as qtg
from socref import util
from socref import abstract
from . import settings
from . import package








@abstract.register_block("Module")
class Block(package.Block):
    """
    This is the module block class. It inherits the python package block. It implements the
    Socrates' Reference abstract block class. It represents a python module.
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
        #
        # Make sure this is a module block before returning its file name.
        #
        if self._TYPE_ == "Module" : return self._p_name + ".py"


    def build(self, def_, begin=""):
        """
        Implements the .package.Block interface.

        def_ : See interface docs.

        begin : See interface docs.

        return : See interface docs.
        """
        #
        # Initialize the source code with the package block's generated source code.
        #
        ret = package.Block.build(self,def_,begin)
        #
        # Add the source code of all this block's children to the returned source code.
        #
        ret += self._build_children_(def_,begin)
        #
        # If this is the special main module then add the special main line to the source code.
        #
        if self._p_name == "__main__" :
            ret += "\n"*settings.H1LINES + 'if __name__ == "__main__" : main()\n'
        #
        # Return the source code.
        #
        return ret
