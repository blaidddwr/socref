"""
Contains the Variable class.
"""
from socref import public as scr
from socref_cpp import block as cppblock








@scr.register("Variable")
class Variable(cppblock.Variable):
    """
    This is the variable block class. It implements the Socrates' Reference
    abstract block class. It represents a C++/Qt variable.
    """
    pass
