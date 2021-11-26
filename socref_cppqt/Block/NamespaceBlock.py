"""
Contains the NamespaceBlock class.
"""
from socref import block
from socref_cpp.Block.NamespaceBlock import NamespaceBlock as CppNamespaceBlock




@block("Namespace",root=True)
class NamespaceBlock(CppNamespaceBlock):
    """
    This is the name space block class. It implements the Socrates' Reference
    abstract block class. It represents a C++/Qt name space.
    """
    pass
