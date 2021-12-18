"""
Contains the EnumerationWriter class.
"""
from ..Base.BlockWriterBase import BlockWriterBase




class EnumerationWriter(BlockWriterBase):
    """
    This is the enumeration writer class. It implements the Socrates' Reference
    abstract writer class. It outputs all code lines for an enumeration
    declaration.
    """


    def __init__(
        self
        ,block
        ,depth
        ,parent
    ):
        """
        Initializes this new enumeration writer with the given enumeration
        block, indentation depth of this writer's output, and parent writer. The
        given enumeration block is used by this writer to find its corresponding
        reader and generate its output. The given parent writer must be a name
        space or class writer.

        Parameters
        ----------
        block : EnumerationBlock
                The enumeration block.
        depth : int
                The indentation depth.
        parent : AbstractWriter
                 The parent writer.
        """
        super().__init__(block,depth,"enum class",parent)
        self.__reader = None


    def _lines_(
        self
    ):
        return self.__reader.lines() if self.__reader else []


    def _link_(
        self
    ):
        self.__reader = self.lookup(self._block_().key())


    def _postLines_(
        self
    ):
        return self.__reader.postLines() if self.__reader else []
