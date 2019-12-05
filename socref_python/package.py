"""
Contains the package block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import util
from socref import abstract
from . import settings
from . import parser








@abstract.register_block("Package",root=True)
class Block(abstract.Block):
    """
    This is the package block class. It implements the Socrates' Reference abstract block class. It
    represents a python package. It implements two additional interface methods used by other block
    definitions of the python language.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new package block.
        """
        abstract.Block.__init__(self)
        self._p_name = ""
        self._p_description = ""


    ##########################
    # PUBLIC - Basic Methods #
    ##########################


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/python/package.svg")


    def display_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return self._p_name


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        description = "</P><P>".join(
            (html.escape(block) for block in self._p_description.split("\n\n") if block)
        )
        if description : description = "<H1>Description</H1><P>%s</P>" % description
        return description


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ("Package","Module")


    #############################
    # PUBLIC - Property Methods #
    #############################


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = []
        ret.append(util.line_edit("Name:","_p_name"))
        ret.append(util.text_edit("Description:","_p_description",speller=True))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        self._p_name = "package"
        self._p_description = "Detailed description."


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        self._p_name = ""
        self._p_description = ""


    ############################
    # PUBLIC - Parsing Methods #
    ############################


    def dir_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        if self._TYPE_ == "Package" : return self._p_name


    def parser(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return parser.Parser(self)


    def space(self, previous):
        """
        This interface is a getter method.

        previous : The block whose source code output is previous to this one.

        return : The number of blank lines that separates this block's source code output from the
                 given previous block's output. The default implementation returns an empty string.
        """
        return ""


    def build(self, definition, begin=""):
        """
        This interface is a getter method.

        definition : Scanned definitions that contains any lines of code scanned from the old source
                     code files.

        begin : An optional string that is appended to every line of the returned source code.

        return : A string of source code lines that is the build output of this block and represents
                 it in the source code. The given scanned definitions and optional begin string are
                 used to generate the lines.
        """
        ret = '"""\n'
        ret += util.wrap_blocks(self._p_description,columns=settings.COLUMNS)
        ret += '"""\n'
        ret += "\n".join(definition.pop("header") + [""])
        return ret


    #######################
    # PROTECTED - Methods #
    #######################


    def _build_children_(self, definition, begin=""):
        """
        Getter method.

        definition : Scanned definitions from the old source code files.

        begin : An optional string that is appended to every line of the returned source code.

        return : A string that is the combined source code of all this block's children's build
                 interface, separated using their space interface.
        """
        ret = ""
        previous = None
        for block in self :
            ret += block.space(previous)
            ret += block.build(definition,begin)
            previous = block
        return ret
