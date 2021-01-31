"""
Contains the ObjectWriter class.
"""
from socref.Abstract.AbstractWriter import *
from socref.Output.Code import *




class ObjectWriter(AbstractWriter):
    """
    Detailed description.
    """


    def __init__(
        self
        ,block
        ,depth
        ,parent
    ):
        """
        asdf

        Parameters
        ----------
        block : 
        depth : 
        parent : 
        """
        super().__init__(parent)
        self.__block = block
        self.__depth = depth


    def _footer_(
        self
    ):
        """
        Detailed description.
        """
        return []


    def _header_(
        self
    ):
        """
        Detailed description.
        """
        ret = Code("    ")
        ret.addBlank(2)
        ret.setDepth(self.__depth)
        ret.add("#")
        ret.addText(self.__block._p_description,80,"# ")
        ret.add("#")
        ret.add(self.__block._p_name+" = "+self.__block._p_assignment)
        return ret
