"""
Contains the ShaderReader class.
"""
from .FunctionReader import *
from re import compile as reCompile
from socref.Base.ReaderBase import *




class ShaderReader(ReaderBase):
    """
    This is the shader reader class. It implements the Socrates' Reference
    abstract reader class. It parses shader files; saving function definitions.
    Parsed function definitions are added as new reader children.
    """
    __functionRE = reCompile("^.*\s+(\w+)\(.*$")


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new shader reader with the given block and parent
        parser.

        Parameters
        ----------
        block : ShaderBlock
                A shader block that is associated with this reader's parsed
                shader file.
        parent : Parser
                 The parser currently parsing its project's source code files.
        """
        super().__init__(parent)
        self._setKey_(block.key())


    def unknown(
        self
    ):
        return []


    def _scan_(
        self
    ):
        while True:
            (i,line) = self.read()
            if line is None:
                break
            match = self.__functionRE.match(line)
            if match:
                name = match.group(1)
                reader = FunctionReader(name,self)
                reader()
                continue
