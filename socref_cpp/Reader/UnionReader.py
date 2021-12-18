"""
Contains the UnionReader class.
"""
from ..Base.BlockReaderBase import BlockReaderBase




class UnionReader(BlockReaderBase):
    """
    This is the union reader class. It implements the Socrates' Reference
    abstract reader class. It parses a union; saving lines of code.
    """


    def _scan_(
        self
    ):
        super()._scanLines_()
        super()._scanPostLines_()
