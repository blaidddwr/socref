"""
Contains the FunctionReader class.
"""
from ..Base.PythonReaderBase import PythonReaderBase




class FunctionReader(PythonReaderBase):
    """
    This is the function reader class. It implements the Socrates' Reference
    abstract reader class. It parses a function; saving all lines of code nested
    within the function definition.
    """


    def __init__(
        self
        ,name
        ,end
        ,parent
    ):
        """
        Initializes this new function reader with the given function name this
        reader will parse, header ending flag, and parent reader that discovered
        the header code line of the function that this reader will parse. The
        given header ending flag is true if this reader's parent read past all
        header lines of the function or false otherwise.

        Parameters
        ----------
        name : string
               The function name.
        end : bool
              The header ending flag.
        parent : AbstractReader
                 The parent reader.
        """
        super().__init__(parent)
        self._setKey_(parent.key()+"."+name)
        self.__end = end
        self.__lines = []


    def lines(
        self
    ):
        """
        Returns lines of code parsed by this function reader. This can only be
        called once, after which an empty list is returned.

        Returns
        -------
        result : list
                 The lines of code or an empty list.
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
        self.__skipEnd_()
        self._skipDocString_()
        self.__scanLines_()


    def __scanLines_(
        self
    ):
        """
        Scans all nested lines of code within this reader's parsed function. The
        indentation is determined by the very first line read, and scanning
        stops when the first line of code lesser than that indent is
        encountered.
        """
        ind = None
        while True:
            self.save()
            (i,line) = self.read()
            if line is None:
                break
            if not line:
                self.__lines.append("")
            else:
                if ind is None:
                    ind = i
                diff = i-ind
                if diff >= 0:
                    self.__lines.append(" "*diff + line)
                else:
                    self.restore()
                    break
            self.discard()
        while self.__lines[-1] == "":
            self.__lines.pop()


    def __skipEnd_(
        self
    ):
        """
        Skips any remaining header code lines of this reader's parsed function.
        """
        if not self.__end:
            while True:
                (i,line) = self.read()
                if not line or line.endswith("):"):
                    break
