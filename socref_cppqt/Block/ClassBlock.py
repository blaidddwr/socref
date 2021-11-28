"""
Contains the ClassBlock class.
"""
from socref import block
from socref_cpp.Block.ClassBlock import ClassBlock as CppClassBlock




@block("Class")
class ClassBlock(CppClassBlock):
    """
    This is the class block class. It implements the Socrates' Reference
    abstract block class. It represents a C++/Qt class.
    """
    pass
