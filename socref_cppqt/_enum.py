"""
Contains the Enum class.
"""
from socref import public as scr
from socref_cpp import block as cppblock








@scr.register("Enumeration")
class Enum(cppblock.Enum):
    """
    This is the enumeration block class. It implements the Socrates' Reference
    abstract block class. It represents a C++/Qt enumeration.
    """
    pass
