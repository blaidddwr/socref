"""
Contains the package block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from socref import abstract
from . import settings
from . import parser








@register("Package",root=True)
class Package(abstract.AbstractBlock):
    """
    This is the package block class. It implements the Socrates' Reference abstract block class. It
    represents a python package. It is the root block type of a python project. It implements two
    additional interface methods used by other block definitions of the python language.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new package block.
        """
        abstract.AbstractBlock.__init__(self)
        self._p_name = ""
        self._p_description = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def build(self, definition, begin=""):
        """
        This interface is a getter method.

        definition : Scanned definitions that contains any lines of code scanned from the old source
                     code files.

        begin : An optional string that is added to the beginning of every line of returned source
                code.

        return : A list of source code lines for this block.
        """
        ret = []
        ret.append('"""')
        ret += ut.wrapBlocks(self._p_description,columns=settings.COLUMNS)
        ret.append('"""')
        ret += definition.pop("header")
        return ret


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Package","Module")


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        self._p_name = ""
        self._p_description = ""


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return self._p_name


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ut.richTextBlocks(1,"Description",html.escape(self._p_description))


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = []
        ret.append(ut.lineEdit("Name:","_p_name"))
        ret.append(ut.textEdit("Description:","_p_description",speller=True))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/python/package.svg")


    def parser(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return parser.Parser(self)


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        self._p_name = "package"
        self._p_description = "Detailed description."


    #######################
    # PROTECTED - Methods #
    #######################


    def _buildChildren_(self, definition, begin=""):
        """
        Getter method.

        definition : Scanned definitions from the old source code files.

        begin : An optional string that is appended to every line of the returned source code.

        return : A list of source code lines that is the combined source code of all this block's
                 children's build interface, separated using their space interface.
        """
        ret = []
        for block in self:
            ret += block.build(definition,begin)
        return ret
