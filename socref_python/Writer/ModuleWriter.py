"""
Contains the ModuleWriter class.
"""
from .ClassWriter import *
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
        block : 
        parent : 
        """
        super().__init__(parent)
        self.__block = block
        self.__reader = self.lookup(block.key(True))
        if block._TYPE_ == "Class":
            ClassWriter(block,0,self)
        else:
            for child in block:
                if child._TYPE_ == "Function":
                    FunctionWriter(child,0,self)
                elif child._TYPE_ == "Class" and child.isInfile():
                    ClassWriter(child,0,self)


    def _footer_(
        self
    ):
        """
        Detailed description.
        """
        ret = Code("    ")
        if self.__reader:
            f = self.__reader.footer()
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
        if self.__reader:
            ret.add(self.__reader.preHeader())
        ret.add('"""')
        if self.__block._TYPE_ == "Class":
            ret.addText("Contains the "+self.__block._p_name+" class.",80)
        else:
            ret.addText(self.__block._p_description,80)
        ret.add('"""')
        if self.__reader:
            ret.add(self.__reader.header())
        return ret
