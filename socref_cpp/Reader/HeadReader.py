"""
Contains the HeadReader class.
"""
from ..Base.CppReaderBase import CppReaderBase




class HeadReader(CppReaderBase):
    """
    This is the head reader class. It implements the Socrates' Reference
    abstract reader class. It parses header files; saving header lines, class
    declarations, and namespace declarations. Parsed class or namespace
    declarations are added as new reader children.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new head reader with the given block and parent parser.
        The given block must be a namespace or class block that is associated
        with this reader's header file.

        Parameters
        ----------
        block : AbstractBlock
                The block.
        parent : Parser
                 The parent parser.
        """
        super().__init__(parent)
        self._setKey_(block.key(True)+".h")
        self.__header = []
        self.__scope = []


    def header(
        self
    ):
        """
        Returns header lines of code parsed after the header guard macro lines
        of this reader's header file. This can only be called once, after which
        an empty list is returned.

        Returns
        -------
        result : list
                 The header lines of code or an empty list.
        """
        ret = self.__header
        self.__header = []
        return ret


    def scope(
        self
    ):
        return self.__scope


    def unknown(
        self
    ):
        return self.__header


    def _scan_(
        self
    ):
        self._skipMacros_(2)
        self.__header = self._scanHeader_()
        self._skipBlanks_()
        self.__scope = self._scanScope_()
        self._scanHeadBody_()
