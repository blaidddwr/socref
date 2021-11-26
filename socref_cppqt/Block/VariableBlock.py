"""
Contains the VariableBlock class.
"""
from socref import block
from socref_cpp.Block.VariableBlock import VariableBlock as CppVariableBlock




@block("Variable")
class VariableBlock(CppVariableBlock):
    """
    This is the variable block class. It implements the Socrates' Reference
    abstract block class. It represents a C++/Qt variable.
    """
    pass
