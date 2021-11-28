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
        self.__lines = []


    def lines(
        self
    ):
        """
        Returns header lines of code parsed by this shader reader. This can only
        be called once, after which an empty list is returned.

        Returns
        -------
        result : list
                 The header lines of code.
        """
        ret = self.__lines
        self.__lines = []
        return ret


    def unknown(
        self
    ):
        return self.__lines


    def _scan_(
        self
    ):
        self.__skipHeader_()
        self.__lines = self.__scanLines_()
        self.__scanFunctions_()


    def __scanFunctions_(
        self
    ):
        """
        Scans for any function definitions. New function readers are created as
        children of this reader for each definition found.
        """
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


    def __scanLines_(
        self
    ):
        """
        Scans for header lines of code and returns them. This stops scanning
        when the first empty line is encountered.

        Returns
        -------
        result : list
                 The header lines of code.
        """
        ret = []
        while True:
            (i,line) = self.read()
            if not line:
                break
            else:
                ret.append(" "*i + line)
        return ret


    def __skipHeader_(
        self
    ):
        """
        Skips the initial header lines of this reader's shader file. This stops
        after the closing multi line comment line of the shader description is
        encountered.
        """
        while True:
            (i,line) = self.read()
            if not line or line == "*/":
                break
