"""
Contains the Variable class.
"""
from socref import register
from socref_cpp import block as cppblock








@register("Variable")
class Variable(cppblock.Variable):
    """
    This is the variable block class. It implements the Socrates' Reference abstract block class. It
    represents a C++/Qt variable.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(
        self
        ):
        """
        Initializes a new variable block.
        """
        cppblock.Variable.__init__(self)
