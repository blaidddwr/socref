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


    def unknown(
        self
    ):
        return self.__lines


    def _scanLines_(
        self
    ):
        """
        Scans for lines of code within a block and returns them, removing
        indentation spaces from each line where the indentation is determined
        from the first line scanned. This stops scanning lines once it
        encounters the final closing bracket of the block. It is assumed the
        very first line read is the first opening bracket, scanning and
        returning nothing if it is not.

        Returns
        -------
        result : list
                 The lines of code.
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
