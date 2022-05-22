"""
Contains the FunctionReader class.
"""
from ..Base.BlockReaderBase import BlockReaderBase




class FunctionReader(BlockReaderBase):
    """
    This is the function reader class. It implements the Socrates' Reference
    abstract reader class. It parses a function; saving regular and
    initialization lines of code.
    """


    def __init__(
        self
        ,name
        ,parent
    ):
        """
        Initializes this new function reader with the given name and parent
        source reader. The given name must have the class name space scope.

        Parameters
        ----------
        name : string
               The name.
        parent : SourceReader
                 The parent source reader.
        """
        super().__init__(None,parent)
        self.__name = "".join(parent.scope()+[name])
        self.__initLines = []


    def initLines(
        self
    ):
        """
        Returns initialization lines of code parsed by this reader. This can
        only be called once, after which an empty list is returned.

        Returns
        -------
        result : list
                 The initialization lines of code or an empty list.
        """
        ret = self.__initLines
        self.__initLines = []
        return ret


    def unknown(
        self
    ):
        ret = []
        if self.__initLines:
            ret += ["INIT:"]+self.__initLines
        lines = super().unknown()
        if lines:
            ret += ["LINES:"]+lines
        return ret


    def _scan_(
        self
    ):
        self._setKey_(self.__name+self.__scanSignature_())
        self.__initLines = self.__scanInitLines_()
        self._scanLines_()


    def __scanInitLines_(
        self
    ):
        """
        Scans for initialization lines of code and returns them, removing
        indentation spaces from each line where the indentation is determined
        from the first line parsed. This stops scanning when the first opening
        bracket line of this reader's function is encountered.

        Returns
        -------
        result : list
                 The initialization lines of code.
        """
        ret = []
        ind = None
        while True:
            self.save()
            (i,line) = self.read()
            if line is None:
                break
            elif line == "{":
                self.restore()
                break
            elif line:
                if ind is None:
                    ind = i
                ret.append(" "*max(0,i-ind) + line)
            self.discard()
        return ret


    def __scanSignature_(
        self
    ):
        """
        Scans for the argument signature of this reader's function, returning it
        with opening and closing parenthesis. This stops scanning when the
        closing parenthesis argument line is encountered.

        Returns
        -------
        result : string
                 The argument signature.
        """
        parts = []
        constant = False
        while True:
            (i,line) = self.read()
            if line is None or line.startswith(")"):
                if line.endswith("const"):
                    constant = True
                break
            elif line:
                s = line.split()
                if len(s) > 1:
                    s = s[:-1]
                parts.append("".join(s))
        return "("+"".join(parts)+(")const" if constant else ")")
