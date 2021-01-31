"""
Contains the Parser class.
"""
from .Reader.ModuleReader import *
from .Writer.ModuleWriter import *
from socref import parser
from socref.Abstract.AbstractParser import *




@parser
class Parser(AbstractParser):
    """
    _DEPRECATED_
    """


    def __init__(
        self
        ,root
    ):
        super().__init__()
        self.__root = root


    def _pathList_(
        self
    ):
        """
        Detailed description.
        """
        def build(path,parent):
            ret = []
            for block in parent:
                if block._TYPE_ == "Module":
                    ret.append((pathJoin(path,block._p_name+".py"),block))
                    ret += build(path,block)
                elif block._TYPE_ == "Package":
                    ret.append((pathJoin(path,block._p_name,"__init__.py"),block))
                    ret += build(pathJoin(path,block._p_name),block)
                elif block._TYPE_ == "Class" and not block.isInfile():
                    ret.append((pathJoin(path,block._p_name+".py"),block))
            return ret
        return build("",self.__root)


    def _reader_(
        self
        ,block
    ):
        """
        Detailed description.

        Parameters
        ----------
        block : 
        """
        if block._TYPE_ == "Module" or block._TYPE_ == "Package" or block._TYPE_ == "Class":
            return ModuleReader(block,self)
        else:
            raise RuntimeError("Unsupported block type given for reader.")


    def _writer_(
        self
        ,block
    ):
        """
        Detailed description.

        Parameters
        ----------
        block : 
        """
        if block._TYPE_ == "Module" or block._TYPE_ == "Package" or block._TYPE_ == "Class":
            return ModuleWriter(block,self)
        else:
            raise RuntimeError("Unsupported block type given for writer.")
