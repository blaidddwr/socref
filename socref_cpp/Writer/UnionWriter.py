"""
Contains the UnionWriter class.
"""
from ..Base.BlockWriterBase import BlockWriterBase




class UnionWriter(BlockWriterBase):
    """
    This is the union writer class. It implements the Socrates' Reference
    abstract writer class. It outputs all code lines for a union declaration.
    """


    def __init__(
        self
        ,block
        ,depth
        ,parent
    ):
        """
        Initializes this new union writer with the given union block,
        indentation depth of this writer's output, and parent writer. The given
        union block is used by this writer to find its corresponding reader and
        generate its output. The given parent writer must be a name space or
        class writer.

        Parameters
        ----------
        block : UnionBlock
                The union block.
        depth : int
                The indentation depth.
        parent : AbstractWriter
                 The parent writer.
        """
        super().__init__(block,depth,"union",parent)
        self.__reader = None


    def _lines_(
        self
    ):
        return self.__reader.lines() if self.__reader else []


    def _link_(
        self
    ):
        self.__reader = self.lookup(self._block_().key())
