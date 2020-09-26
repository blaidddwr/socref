"""
Contains the Namespace class.
"""
from socref import public as scr
from socref_cpp import block as cppblock








@scr.register("Namespace",root=True)
class Namespace(cppblock.Namespace):
    """
    This is the namespace block class. It implements the Socrates' Reference
    abstract block class. It represents a C++/Qt namespace. It is the root block
    type of a C++/Qt project.
    """
    pass
