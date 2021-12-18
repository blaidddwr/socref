"""
Contains the BlockReaderBase class.
"""
from .CppReaderBase import CppReaderBase




class BlockReaderBase(CppReaderBase):
    """
    This is the block reader base class. It partially implements the Socrates'
    Reference abstract reader class. It provides common methods used by readers
    that scan lines of code within a curly bracket block and get those parsed
    lines. This class can also optionally sets its lookup key.
    """


    def __init__(
        self
        ,name
        ,parent
    ):
        """
        Initializes this new block reader base with the given name and parent
        reader. The given reader's scope and the given name is used to generate
        this reader's lookup key. If the given name is nothing then this does
        not set its lookup key.

        Parameters
        ----------
        name : string
               The name or nothing.
        parent : CppReaderBase
                 The parent reader.
        """
        super().__init__(parent)
        if name is not None:
            self._setKey_("::".join(parent.scope()+[name]))
        self.__lines = []
        self.__postLines = []


    def lines(
        self
    ):
        """
        Returns lines of code parsed by this reader. This can only be called
        once, after which an empty list is returned.

        Returns
        -------
        result : list
                 The lines of code or an empty list.
        """
        ret = self.__lines
        self.__lines = []
        return ret


    def postLines(
        self
    ):
        """
        Returns post lines of code parsed by this reader. This can only be
        called once, after which an empty list is returned.

        Returns
        -------
        result : list
                 The post lines of code or an empty list.
        """
        ret = self.__postLines
        self.__postLines = []
        return ret


    def unknown(
        self
    ):
        return self.__lines


    def _scanLines_(
        self
    ):
        """
        Scans for lines of code within a block, removing indentation spaces from
        each line where the indentation is determined from the first line
        scanned. This stops scanning lines once it encounters the final closing
        bracket of the block. It is assumed the very first line read is the
        first opening bracket, scanning and returning nothing if it is not.
        """
        self.save()
        (i,line) = self.read()
        if line != "{":
            self.restore()
        else:
            self.discard()
            d = 1
            ind = None
            while True:
                (i,line) = self.read()
                if line is None:
                    break
                else:
                    d += line.count("{")-line.count("}")
                    if d < 1:
                        break
                    if ind is None:
                        ind = i
                    self.__lines.append(" "*max(0,i-ind) + line)


    def _scanPostLines_(
        self
    ):
        """
        Scans for post lines of code directly after a block, removing
        indentation spaces from each line where the indentation is determined
        from the first line scanned. This stops scanning lines once it
        encounters a blank line or a closing bracket.
        """
        ind = None
        while True:
            (i,line) = self.read()
            if not line or line == "}":
                break
            if ind is None:
                ind = i
            self.__postLines.append(" "*max(0,i-ind) + line)
