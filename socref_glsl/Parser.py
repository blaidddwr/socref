"""
Contains the Parser class.
"""
from .Writer.ProgramWriter import *
from socref import parser
from socref.Abstract.AbstractParser import *




@parser
class Parser(AbstractParser):
    """
    This is the parser class. It implements the Socrates' Reference abstract
    parser class for the GLSL language.
    """


    def __init__(
        self
        ,root
    ):
        """
        Initializes this new parser with the given root block.

        Parameters
        ----------
        root : socref_glsl.Block.ProgramBlock
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
                if block._TYPE_ == "Program":
                    ret.append((pathJoin(path,block._p_name,"README.txt"),block))
                    ret += build(pathJoin(path,block._p_name),block)
            return ret
        return build("",self.__root)


    def _reader_(
        self
        ,block
    ):
        pass


    def _writer_(
        self
        ,block
    ):
        return ProgramWriter(block,self)
