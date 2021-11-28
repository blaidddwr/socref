"""
Contains the Parser class.
"""
from .Reader.ShaderReader import ShaderReader
from .Settings import Settings
from .Writer.ProgramWriter import ProgramWriter
from .Writer.ShaderWriter import ShaderWriter
from os.path import join as pathJoin
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
               The root block.
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
                    ret.append(
                        (pathJoin(path,block._p_name,"README.txt"),None,ProgramWriter(block,self))
                    )
                    ret += build(pathJoin(path,block._p_name),block)
                elif block._TYPE_ == "Shader":
                    ret.append(
                        (
                            pathJoin(path,block._p_name+Settings.EXT[block._p_type]+".glsl")
                            ,ShaderReader(block,self)
                            ,ShaderWriter(block,self)
                        )
                    )
            return ret
        return build("",self.__root)
