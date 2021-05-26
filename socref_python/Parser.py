"""
Contains the Parser class.
"""
from .Reader.ModuleReader import *
from .Writer.ModuleWriter import *
from socref import parser
from socref.Base.ParserBase import *




@parser
class Parser(ParserBase):
    """
    This is the parser class. It implements the Socrates' Reference abstract
    parser class for the python language.
    """


    def __init__(
        self
        ,root
    ):
        """
        Initializes this new parser with the given root block.

        Parameters
        ----------
        root : PackageBlock
               The root block of the project this parser will use for generating
               paths, readers, and writers.
        """
        super().__init__()
        self.__root = root


    def _pathList_(
        self
    ):
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
        if block._TYPE_ == "Module" or block._TYPE_ == "Package" or block._TYPE_ == "Class":
            return ModuleReader(block,self)


    def _writer_(
        self
        ,block
    ):
        if block._TYPE_ == "Module" or block._TYPE_ == "Package" or block._TYPE_ == "Class":
            return ModuleWriter(block,self)
        else:
            raise RuntimeError("Unsupported block type given for writer.")
