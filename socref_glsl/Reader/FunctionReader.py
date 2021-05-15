"""
Contains the FunctionReader class.
"""
from socref.Abstract.AbstractReader import *




class FunctionReader(AbstractReader):
    """
    This is the function reader class. It implements the Socrates' Reference
    abstract reader class. It parses a function; saving all lines of code nested
    within the function definition.
    """


    def __init__(
        self
        ,name
        ,parent
    ):
        """
        Initializes this new function reader with the given function name and
        parent reader.

        Parameters
        ----------
        name : string
               The name of the function this reader will parse.
        parent : socref_glsl.Reader.ShaderReader
                 The parent shader reader that discovered the header code line
                 of the function that this reader will parse.
        """
        super().__init__(parent)
        self.__key = parent.key()+"."+name
        self.__signature = ""
        self.__lines = []


    def lines(
        self
    ):
        """
        Getter method. This can only be called once, after which an empty list
        is returned.

        Returns
        -------
        result : list
                 Lines of code contained by the parsed function of this reader.
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
        self.__scanSignature_()
        self._setKey_(self.__key+"("+self.__signature+")")
        self.__scanLines_()


    def __scanLines_(
        self
    ):
        """
        Scans all nested lines of code within this reader's parsed function. The
        beginning and end is determined by opening and closing brackets on their
        own line. The very first opening bracket line and very last closing
        bracket line are not included.
        """
        depth = 1
        while True:
            (i,line) = self.read()
            if line is None:
                break
            elif line == "{":
                depth += 1
            elif line == "}":
                 depth -= 1
            if depth <= 0:
                break
            self.__lines.append(" "*i + line)


    def __scanSignature_(
        self
    ):
        """
        Scans the signature, if any, of this reader's parsed function. If the
        function has no arguments that signature is an empty string.
        """
        sig = []
        while True:
            (i,line) = self.read()
            if line is None or line == "{":
                break
            if line and line != ")":
                sig.append(line[:line.rfind(" ")].replace(" ",""))
        self.__signature = "".join(sig)
