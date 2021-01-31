"""
Contains the ClassWriter class.
"""
from .FunctionWriter import *
from socref.Abstract.AbstractWriter import *
from socref.Output.Code import *




class ClassWriter(AbstractWriter):
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
        block : object
                Detailed description.
        depth : object
                Detailed description.
        parent : object
                 Detailed description.
        """
        super().__init__(parent)
        self.__block = block
        self.__depth = depth
        self.__reader = self.lookup(block.key(False))
        for child in block:
            if child._TYPE_ == "Function":
                FunctionWriter(child,depth+1,self)
            elif child._TYPE_ == "Class":
                ClassWriter(child,depth+1,self)


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
        if self.__block.parent()._TYPE_ == "Class":
            ret.addBlank(2)
        else:
            ret.addBlank(4)
        ret.setDepth(self.__depth)
        ret.add("class "+self.__block._p_name+"("+",".join(self.__block.clsParents())+"):")
        ret.setDepth(self.__depth+1)
        ret.add('"""')
        ret.addText(self.__block._p_description,80)
        ret.add('"""')
        if self.__reader:
            ret.add(self.__reader.lines())
        elif len(self.__block) == 0:
            ret.add("pass")
        return ret
