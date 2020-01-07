"""
Contains classes for namespace, enumeration, enumeration value, template, variable, and union
blocks. All contain block classes are exact copies of their C++ counterparts and require no
modification for Qt.
"""
from socref import register
from socref_cpp import namespace
from socref_cpp import enumeration
from socref_cpp import enumeration_value
from socref_cpp import template
from socref_cpp import variable
from socref_cpp import union








@register("Namespace",root=True)
class Namespace(namespace.Namespace):
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
        namespace.Namespace.__init__(self)








@register("Enumeration")
class Enum(enumeration.Enum):
    """
    This is the enumeration block class. It implements the Socrates' Reference abstract block class.
    It represents a C++/Qt enumeration.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new enumeration block.
        """
        enumeration.Enum.__init__(self)








@register("Enumeration Value")
class EnumValue(enumeration_value.EnumValue):
    """
    This is the enumeration value block class. It implements the Socrates' Reference abstract block
    class. It represents a C++/Qt enumeration value.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new enumeration value block.
        """
        enumeration_value.EnumValue.__init__(self)








@register("Template")
class Template(template.Template):
    """
    This is the template block class. It implements the Socrates' Reference abstract block class. It
    represents a C++/Qt template.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new namespace block.
        """
        template.Template.__init__(self)








@register("Variable")
class Variable(variable.Variable):
    """
    This is the variable block class. It implements the Socrates' Reference abstract block class. It
    represents a C++/Qt variable.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new variable block.
        """
        variable.Variable.__init__(self)








@register("Union")
class Union(union.Union):
    """
    This is the union block class. It implements the Socrates' Reference abstract block class. It
    represents a C++/Qt union.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new union block.
        """
        union.Union.__init__(self)
