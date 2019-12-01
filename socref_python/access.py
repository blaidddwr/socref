"""
Contains the access block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import util
from socref import abstract
from . import settings
from . import package
from . import function








@abstract.register_block("Access")
class Block(package.Block):
    """
    This is the access block class. It inherits the python package block. It implements the
    Socrates' Reference abstract block class. It represents a fictitious access declaration for
    python classes.

    It represents a fictitious access declaration for python classes. Since python does not actually
    have any kind of access control, this is simply displayed as a comment inside of the class
    source code. It is mostly helpful in the reference application's project tree view for
    organizing different parts of a class.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new access block.
        """
        abstract.Block.__init__(self)
        #
        # Initialize this block's properties.
        #
        self._p_name = ""
        self._p_type = "Public"


    ##########################
    # PUBLIC - Basic Methods #
    ##########################


    def has_abstract(self):
        """
        Getter method.

        return : True if this access block contains abstract methods or false otherwise.
        """
        #
        # Iterate through all children of this access block, returning true if any of the methods
        # are abstract.
        #
        for block in self :
            if isinstance(block,function.Block) :
                if block.is_abstract() : return True
        #
        # No child abstract method was found so return false.
        #
        return False


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        #
        # Return the appropriate icon based off this block's properties.
        #
        if self._p_type == "Public" : return qtg.QIcon(":/python/public.svg")
        elif  self._p_type == "Protected" : return qtg.QIcon(":/python/protected.svg")
        else:  return qtg.QIcon(":/python/private.svg")


    def display_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return self._p_name + " (%i)" % len(self)


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        #
        # Return simple rich text that displays this access blocks type and name.
        #
        type_ = html.escape(self._p_type.upper())
        name = html.escape(self._p_name)
        return "<h1>%s</h1><p>%s</p>" % (type_,name)


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ("Object","Function")


    def is_volatile_above(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return True


    #############################
    # PUBLIC - Property Methods #
    #############################


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        #
        # Initialize an empty list. Add the name and then the type edit definitions.
        #
        ret = []
        ret.append(util.line_edit("Name:","_p_name"))
        combo = util.combobox_edit("Type:","_p_type")
        util.add_combo_select(combo,"Public",icon=qtg.QIcon(":/python/public.svg"))
        util.add_combo_select(combo,"Protected",icon=qtg.QIcon(":/python/protected.svg"))
        util.add_combo_select(combo,"Private",icon=qtg.QIcon(":/python/private.svg"))
        ret.append(combo)
        #
        # Return the list.
        #
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        self._p_name = "access"
        self._p_type = "Public"


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        self._p_name = ""
        self._p_type = "Public"


    ############################
    # PUBLIC - Parsing Methods #
    ############################


    def space(self, previous):
        """
        Implements the .package.Block interface.

        previous : See interface docs.

        return : See interface docs.
        """
        return "\n"*settings.H3LINES


    def build(self, def_, begin=""):
        """
        Implements the .package.Block interface.

        def_ : See interface docs.

        begin : See interface docs.

        return : See interface docs.
        """
        #
        # Initialize the line of code that displays this access block's type and name.
        #
        line = "# %s - %s #" % (self._p_type.upper(),self._p_name)
        #
        # Add the comment header of this access block to the source code.
        #
        ret = begin + "#"*len(line) + "\n"
        ret += begin + line + "\n"
        ret += begin + "#"*len(line) + "\n"
        #
        # Add the source code of all this block's children to the returned source code.
        #
        ret += self._build_children_(def_,begin)
        #
        # Return the source code.
        #
        return ret
