"""
Detailed description.
"""
from PySide2 import QtGui as qtg
from socref import util
from socref import abstract
from . import package
from . import settings








class Block(package.Block):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self):
        """
        Detailed description.
        """
        package.Block.__init__(self)
        #


    ####################
    # PUBLIC - Methods #
    ####################


    def file_name(self):
        """
        Detailed description.

        return : Yes
        """
        if self._TYPE_ == "Module" : return self._p_name + ".py"


    def icon(self):
        """
        Detailed description.

        return : Yes
        """
        return qtg.QIcon(":/python/module.svg")


    def build_list(self):
        """
        Detailed description.

        return : Yes
        """
        return ("Object","Function","Class")


    def set_default_properties(self):
        """
        Detailed description.
        """
        package.Block.set_default_properties(self)
        self._p_name = "module"








@abstract.register_block("Module")
class Builder(Block):
    """
    Detailed description.
    """


    ####################
    # PUBLIC - Methods #
    ####################


    def build(self, def_):
        """
        Detailed description.

        def_ : Detailed description.

        return : Yes
        """
        if self._TYPE_ != "Module" : return
        ret = '"""\n'
        ret += util.wrap_blocks(self._p_description,columns=settings.COLUMNS)
        ret += '"""\n'
        ret += "\n".join(def_["header"] + [""])
        previous = None
        for block in self :
            ret += block.space(previous,self)
            ret += block.build(def_)
            previous = block
        return ret
