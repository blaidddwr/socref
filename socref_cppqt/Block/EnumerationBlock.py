"""
Contains the EnumerationBlock class.
"""
from socref import block
from socref_cpp.Block.EnumerationBlock import EnumerationBlock as CppEnumerationBlock




@block("Enumeration")
class EnumerationBlock(CppEnumerationBlock):
    """
    This is the enumeration block class. It implements the Socrates' Reference
    abstract block class. It represents a C++/Qt enumeration.
    """
    pass
