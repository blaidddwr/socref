"""
Contains the Template class.
"""
from socref import register
from socref_cpp import block as cppblock








@register("Template")
class Template(cppblock.Template):
    """
    This is the template block class. It implements the Socrates' Reference
    abstract block class. It represents a C++/Qt template.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(
        self
        ):
        """
        Initializes a new namespace block.
        """
        cppblock.Template.__init__(self)
