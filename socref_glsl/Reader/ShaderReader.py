"""
Contains the ShaderReader class.
"""
from .FunctionReader import FunctionReader
from re import compile as reCompile
from socref.Base.ReaderBase import ReaderBase




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
        Initializes this new shader reader with the given shader block that is
        associated with this reader's source file and parent parser.

        Parameters
        ----------
        block : ShaderBlock
                A shader block.
        parent : Parser
                 The parent parser.
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
