"""
Contains the SourceReader class.
"""
from socref.Base.ReaderBase import ReaderBase




class SourceReader(ReaderBase):
    """
    This is the source reader class. It implements the Socrates' Reference
    abstract reader class. It parses source files; saving header lines and
    function definitions. Parsed function definitions are added as new reader
    children.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new source reader with the given block and parent
        parser. The given block must be a namespace or class block that is
        associated with this reader's source file.

        Parameters
        ----------
        block : AbstractBlock
                The block.
        parent : Parser
                 The parent parser.
        """
        super().__init__(parent)
        self._setKey_(block.key(True)+".cpp")
        self.__header = []


    def header(
        self
    ):
        """
        Returns header lines of code parsed after the initial self header
        include line of this reader's source file. This can only be called once,
        after which an empty list is returned.

        Returns
        -------
        result : list
                 The header lines of code or an empty list.
        """
        ret = self.__header
        self.__header = []
        return ret


    def unknown(
        self
    ):
        return self.__header


    def _scan_(
        self
    ):
        m = 0
        while True:
            (i,line) = self.read()
            if not line:
                return
            if m >= 1:
                self.__header.append(" "*i + line)
            elif line.startswith("#"):
                m += 1
