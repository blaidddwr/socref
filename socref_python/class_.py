"""
Contains the class block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import util
from socref import abstract
from . import settings
from . import package
from . import function








@abstract.register_block("Class")
class Block(function.Descriptor):
    """
    This is the class block class. It inherits the python descriptor class. It implements the
    Socrates' Reference abstract block class. It represents a python class.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new class block.
        """
        function.Descriptor.__init__(self)
        #
        # Initialize this block's properties.
        #
        self._p_parents = ""


    ##########################
    # PUBLIC - Basic Methods #
    ##########################


    def is_abstract(self):
        """
        Getter method.

        return : True if this class contains any abstract functions for false otherwise.
        """
        for access in self :
            if access.has_abstract() : return True
        return False


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        #
        # Return the appropriate icon based off this block's properties.
        #
        if self.is_abstract() : return qtg.QIcon(":/python/abstract_class.svg")
        else: return qtg.QIcon(":/python/class.svg")


    def display_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        #
        # Return this block's name along with its descriptors decoration.
        #
        return self._p_name + self._descriptors_name_()


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        #
        # Generate the rich text parents view of this block.
        #
        parents = "</li><li>".join((parent for parent in self._p_parents.split("\n") if parent))
        if parents : parents = "<h2>Parents</h2><ul><li>%s</li></ul>" % parents
        #
        # Return the description, parents view, and descriptors view.
        #
        return package.Block.display_view(self) + parents + self._descriptors_view_()


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ("Access",)


    #############################
    # PUBLIC - Property Methods #
    #############################


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        #
        # Initialize a list with the inherited package block's edit definitions.
        #
        ret = package.Block.edit_definitions(self)
        #
        # Add the parents edit definition and then the descriptors edit definition.
        #
        ret.append(util.text_edit("Parents:","_p_parents"))
        ret.append(self._descriptors_edit_definition_())
        #
        # Return the list.
        #
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        function.Descriptor.set_default_properties(self)
        self._p_name = "class"
        self._p_parents = ""


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        function.Descriptor.clear_properties(self)
        self._p_parents = ""


    ############################
    # PUBLIC - Parsing Methods #
    ############################


    def space(self, previous):
        """
        Implements the .package.Block interface.

        previous : See interface docs.

        return : See interface docs.
        """
        return "\n"*settings.H1LINES


    def build(self, def_, begin=""):
        """
        Implements the .package.Block interface.

        def_ : See interface docs.

        begin : See interface docs.

        return : See interface docs.
        """
        #
        # Get the scanned dictionary definition of this class, setting it to a blank definition if
        # there is none.
        #
        def_ = def_["classes"].get(self._p_name,{"functions":{}})
        #
        # Add this block's descriptors to the source code.
        #
        ret = self._build_descriptors_(begin)
        #
        # Add the class line of this block to the source code.
        #
        ret += "%sclass %s(%s):\n" % (begin,self._p_name,self.__build_parents_())
        #
        # Add the doc string of this class block to the source code.
        #
        ret += begin + " " * settings.INDENT + '"""\n'
        ret += util.wrap_blocks(self._p_description
                                ,begin=begin + " " * settings.INDENT
                                ,columns=settings.COLUMNS)
        ret += begin + " " * settings.INDENT + '"""\n'
        #
        # Add the source code of all this block's children to the returned source code.
        #
        ret += self._build_children_(def_,begin + " " * settings.INDENT)
        #
        # Return the source code.
        #
        return ret


    #####################
    # PRIVATE - Methods #
    #####################


    def __build_parents_(self):
        """
        Getter method.

        return : A string that is the source code fragment of this class block's parents used in its
                 source code header line.
        """
        return ", ".join((parent for parent in self._p_parents.split("\n") if parent))
