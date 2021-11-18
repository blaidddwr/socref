"""
Contains the NamespaceReader class.
"""
from ..Base.CppReaderBase import CppReaderBase




class NamespaceReader(CppReaderBase):
    """
    This is the namespace reader class. It implements the Socrates' Reference
    abstract reader class. It parses a namespace; saving custom header lines of
    code, namespace declarations, and class declarations, enumeration
    declarations, and union declarations. Parsed namespace, class, enumeration,
    and union declarations are added as new reader children.
    """


    def __init__(
        self
        ,name
        ,parent
    ):
        """
        Initializes this new namespace reader with the given namespace name this
        reader will parse and parent reader that discovered the header code line
        of the namespace that this reader will parse. The given reader must be a
        head or namespace reader.

        Parameters
        ----------
        name : string
               The name.
        parent : AbstractReader
                 The parent reader.
        """
        super().__init__(parent)
        self.__scope = parent.scope()+[name]
        self._setKey_("::".join(self.__scope))
        self.__lines = []


    def lines(
        self
    ):
        """
        Returns lines of custom header code parsed by this namespace reader.
        This can only be called once, after which an empty list is returned.

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
            self._scanHeadBody_()
        else:
            self.restore()
