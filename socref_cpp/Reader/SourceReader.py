"""
Contains the SourceReader class.
"""
from ..Base.FileReaderBase import FileReaderBase
from ..Settings import Settings




class SourceReader(FileReaderBase):
    """
    This is the source reader class. It implements the Socrates' Reference
    abstract reader class. It parses source files; saving macros, header lines,
    and definitions. Parsed definitions are added as new reader children.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new source reader with the given block and parent
        parser. The given block must be a name space or class block that is
        associated with this reader's source file.

        Parameters
        ----------
        block : AbstractBlock
                The block.
        parent : Parser
                 The parent parser.
        """
        super().__init__(parent)
        self._setKey_(block.key(True)+Settings.SRC_EXT)


    def _scan_(
        self
    ):
        self._skipMacros_(1)
        super()._scan_()
