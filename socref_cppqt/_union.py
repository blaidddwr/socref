"""
Contains the Union class.
"""
from socref import register
from socref_cpp import block as cppblock








@register("Union")
class Union(cppblock.Union):
    """
    This is the union block class. It implements the Socrates' Reference
    abstract block class. It represents a C++/Qt union.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(
        self
        ):
        """
        Initializes a new union block.
        """
        cppblock.Union.__init__(self)
