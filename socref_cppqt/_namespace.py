"""
Contains the Namespace class.
"""
from socref import register
from socref_cpp import block as cppblock








@register("Namespace",root=True)
class Namespace(cppblock.Namespace):
    """
    This is the namespace block class. It implements the Socrates' Reference abstract block class.
    It represents a C++/Qt namespace. It is the root block type of a C++/Qt project.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new namespace block.
        """
        cppblock.Namespace.__init__(self)
