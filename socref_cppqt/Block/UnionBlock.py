"""
Contains the UnionBlock class.
"""
from socref import block
from socref_cpp.Block.UnionBlock import UnionBlock as CppUnionBlock




@block("Union")
class UnionBlock(CppUnionBlock):
    """
    This is the union block class. It implements the Socrates' Reference
    abstract block class. It represents a C++/Qt union.
    """
    pass
