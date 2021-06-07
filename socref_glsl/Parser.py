"""
Contains the Parser class.
"""
from .Reader.ShaderReader import ShaderReader
from .Writer.ProgramWriter import ProgramWriter
from .Writer.ShaderWriter import ShaderWriter
from socref import parser
from socref.Base.ParserBase import ParserBase




@parser
class Parser(ParserBase):
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
        root : ProgramBlock
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
                elif block._TYPE_ == "Shader":
                    ret.append(
                        (pathJoin(path,block._p_name+Settings.EXT[block._p_type]+".glsl"),block)
                    )
            return ret
        return build("",self.__root)


    def _reader_(
        self
        ,block
    ):
        if block._TYPE_ == "Shader":
            return ShaderReader(block,self)


    def _writer_(
        self
        ,block
    ):
        if block._TYPE_ == "Program":
            return ProgramWriter(block,self)
        elif block._TYPE_ == "Shader":
            return ShaderWriter(block,self)
        else:
            raise RuntimeError("Unsupported block type given for writer.")
