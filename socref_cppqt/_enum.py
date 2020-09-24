"""
Contains the Enum class.
"""
from socref import register
from socref_cpp import block as cppblock








@register("Enumeration")
class Enum(cppblock.Enum):
    """
    This is the enumeration block class. It implements the Socrates' Reference
    abstract block class. It represents a C++/Qt enumeration.
    """


    def __init__(
        self
        ):
        """
        Initializes a new enumeration block.
        """
        super().__init__()
