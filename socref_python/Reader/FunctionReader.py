"""
Contains the FunctionReader class.
"""
from . import skipDocString
from socref.Base.ReaderBase import ReaderBase




class FunctionReader(ReaderBase):
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
        Initializes this new function reader with the given function name,
        header ending flag, and parent reader.

        Parameters
        ----------
        name : string
               The name of the function this reader will parse.
        end : bool
              True if this writer's parent reader read past all header lines of
              its to be parsed function or false otherwise.
        parent : AbstractReader
                 The parent reader that discovered the header code line of the
                 function that this reader will parse.
        """
        super().__init__(parent)
        self._setKey_(parent.key()+"."+name)
        self.__end = end
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
        self.__skipEnd_()
        skipDocString(self)
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
