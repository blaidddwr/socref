"""
Contains the ClassReader class.
"""
from ..Base.CppReaderBase import CppReaderBase




class ClassReader(CppReaderBase):
    """
    This is the class reader class. It implements the Socrates' Reference
    abstract reader class. It parses a class; saving custom header lines of
    code, enumeration declarations, and union declarations. Parsed enumeration
    and union declarations are added as new reader children.
    """


    def __init__(
        self
        ,name
        ,parent
    ):
        """
        Initializes this new class reader with the given class name this reader
        will parse and parent head reader that discovered the header code line
        of the class that this reader will parse.

        Parameters
        ----------
        name : string
               The name.
        parent : HeadReader
                 The head reader.
        """
        super().__init__(parent)
        self.__scope = parent.scope()+[name]
        self._setKey_("::".join(self.__scope))
        self.__lines = []


    def lines(
        self
    ):
        """
        Returns lines of custom header code parsed by this reader. This can only
        be called once, after which an empty list is returned.

        Returns
        -------
        result : list
                 The lines of code or an empty list.
        """
        ret = self.__lines
        self.__lines = []
        return ret


    def scope(
        self
    ):
        return self.__scope


    def unknown(
        self
    ):
        return self.__lines


    def _scan_(
        self
    ):
        self.save()
        (i,line) = self.read()
        if line == "{":
            self.discard()
            self.__lines = self._scanHeader_()
            self._scanDeclarations_()
        else:
            self.restore()
