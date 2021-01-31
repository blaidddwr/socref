"""
Contains the ModuleWriter class.
"""
from .FunctionWriter import *
from socref.Abstract.AbstractWriter import *
from socref.Output.Code import *




class ModuleWriter(AbstractWriter):
    """
    Detailed description.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        asdf

        Parameters
        ----------
        block : object
                Detailed description.
        parent : object
                 Detailed description.
        """
        super().__init__(parent)
        self.__b = block
        self.__r = self.lookup(block.key())
        for child in block:
            if child._TYPE_ == "Function":
                FunctionWriter(child,0,self)


    def _footer_(
        self
    ):
        """
        Detailed description.
        """
        ret = Code("    ")
        if self.__r:
            f = self.__r.footer()
            if f:
                ret.addBlank(2)
                ret.add(f)
        return ret


    def _header_(
        self
    ):
        """
        Detailed description.
        """
        ret = Code("    ")
        if self.__r:
            ret.add(self.__r.preHeader())
        ret.add('"""')
        ret.addText(self.__b._p_description,80)
        ret.add('"""')
        if self.__r:
            ret.add(self.__r.header())
        return ret
