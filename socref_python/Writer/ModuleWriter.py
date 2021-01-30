"""
Contains the ModuleWriter class.
"""
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
        self.setLookupBase(block.key())
        self.__b = block


    def _footer_(
        self
    ):
        """
        Detailed description.
        """
        ret = Code()
        r = self.lookup()
        if r:
            footer = r.footer()
            if footer:
                ret.add([""]*2 + r.footer())
        return ret


    def _header_(
        self
    ):
        """
        Detailed description.
        """
        ret = Code()
        r = self.lookup()
        if r:
            ret.add(r.preHeader())
            ret.add('"""')
            ret.addText(self.__b._p_description,80)
            ret.add('"""')
            ret.add(r.header())
        return ret
