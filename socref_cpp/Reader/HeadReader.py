"""
Contains the HeadReader class.
"""
from ..Base.FileReaderBase import FileReaderBase
from ..Settings import Settings




class HeadReader(FileReaderBase):
    """
    This is the head reader class. It implements the Socrates' Reference
    abstract reader class. It parses header files; saving macros, header lines,
    and declarations. Parsed declarations are added as new reader children.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new head reader with the given block and parent parser.
        The given block must be a name space or class block that is associated
        with this reader's header file.

        Parameters
        ----------
        block : AbstractBlock
                The block.
        parent : Parser
                 The parent parser.
        """
        super().__init__(parent)
        self._setKey_(block.key()+Settings.HDR_EXT)
        self.__block = block


    def scope(
        self
    ):
        return (
            super().scope()
            + (
                [self.__block._p_name]
                if self.__block._TYPE_ == "Namespace" and self.__block.parent()
                else []
            )
        )


    def _scan_(
        self
    ):
        self._skipMacros_(2)
        super()._scan_()
        self._scanDeclarations_()
