"""
Contains the EnumerationReader class.
"""
from ..Base.BlockReaderBase import BlockReaderBase




class EnumerationReader(BlockReaderBase):
    """
    This is the enumeration reader class. It implements the Socrates' Reference
    abstract reader class. It parses an enumeration; saving lines of code.
    """


    def _scan_(
        self
    ):
        super()._scanLines_()
