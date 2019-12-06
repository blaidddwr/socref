"""
Contains the class block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from socref import abstract
from . import settings
from . import package
from . import function








@register("Class")
class Block(function.Descriptor):
    """
    This is the class block class. It implements the Socrates' Reference abstract block class. It
    represents a python class.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new class block.
        """
        function.Descriptor.__init__(self)
        self._p_parents = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def is_abstract(self):
        """
        Getter method.

        return : True if this class contains any abstract functions for false otherwise.
        """
        for access in self:
            if access.has_abstract():
                return True
        return False


    ##########################
    # PUBLIC - Basic Methods #
    ##########################


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        if self.is_abstract():
            return qtg.QIcon(":/python/abstract_class.svg")
        else:
            return qtg.QIcon(":/python/class.svg")


    def display_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return self._p_name + self._descriptors_name_()


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        parents = ut.rich_text(
            2
            ,"Parents"
            ,"".join(("<li>%s</li>" % parent for parent in self._p_parents.split("\n") if parent))
        )
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
        ret = package.Block.edit_definitions(self)
        ret.append(ut.text_edit("Parents:","_p_parents"))
        ret.append(self._descriptors_edit_definition_())
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


    def build(self, definition, begin=""):
        """
        Implements the .package.Block interface.

        definition : See interface docs.

        begin : See interface docs.

        return : See interface docs.
        """
        #
        # Get the scanned dictionary definition of this class.
        #
        definition = definition["classes"].get(self._p_name,{"functions":{}})
        ret = self._build_descriptors_(begin)
        ret += "%sclass %s(%s):\n" % (begin,self._p_name,self.__build_parents_())
        ret += begin + " "*settings.INDENT + '"""\n'
        ret += ut.wrap_blocks(
            self._p_description
            ,begin=begin + " "*settings.INDENT
            ,columns=settings.COLUMNS
        )
        ret += begin + " "*settings.INDENT + '"""\n'
        ret += self._build_children_(definition,begin + " " * settings.INDENT)
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
