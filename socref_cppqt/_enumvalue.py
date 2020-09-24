"""
Contains the EnumValue class.
"""
from socref import register
from socref_cpp import block as cppblock








@register("Enumeration_Value")
class EnumValue(cppblock.EnumValue):
    """
    This is the enumeration value block class. It implements the Socrates'
    Reference abstract block class. It represents a C++/Qt enumeration value.
    """


    def __init__(
        self
        ):
        """
        Initializes a new enumeration value block.
        """
        super().__init__()
