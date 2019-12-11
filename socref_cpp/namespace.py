"""
Contains the namespace block definition and generic base block.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from socref import abstract
from . import parser








class Base(abstract.AbstractBlock):
    """
    This is the base block class. It implements the Socrates' Reference abstract block class. It is
    a base block class the many other C++ blocks use which contains name and description properties.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new namespace block.
        """
        abstract.AbstractBlock.__init__(self)
        self._p_name = ""
        self._p_description = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def buildDeclaration(self, begin):
        """
        This interface is a getter method.

        begin : A string that is added to the beginning of each returned line of declaration code.

        return : A list of source code lines that is the declaration of this block.
        """
        return []


    def buildDefinition(self, definitions, scope, template, begin):
        """
        This interface is a getter method.

        definitions : A dictionary containing scanned lines of function code where the key is a
                      function's signature.

        scope : A string that is the scope for this block, not including the final double colon
                characters.

        template : A string that is any template declarations that this block is within.

        begin : A string that is added to the beginning of every returned line of code.

        return : A list of source code lines that is the definition of this block.
        """
        return []


    def buildHeader(self, definitions):
        """
        This interface is a getter method.

        definitions : A dictionary containing scanned lines of function code where the key is a
                      function's signature.

        return : A list of source code lines that is the entire header file this block represents.
                 If this block does not represent a header file then an empty list is returned.
        """
        return []


    def buildSource(self, definitions):
        """
        This interface is a getter method.

        definitions : A dictionary containing scanned lines of function code where the key is a
                      function's signature.

        return : A list of source code lines that is the entire source file this block represents.
                 If this block does not represent a source file then an empty list is returned.
        """
        return []


    def buildTemplate(self, definitions, scope, template, begin):
        """
        This interface is a getter method.

        definitions : A dictionary containing scanned lines of function code where the key is a
                      function's signature.

        scope : A string that is the scope for this block, not including the final double colon
                characters.

        template : A string that is any template declarations that this block is within.

        begin : A string that is added to the beginning of every returned line of code.

        return : A list of source code lines that is the template definition of this block. This
                 means definitions that must exist in the header file due to the block being part of
                 a template.
        """
        return ([],[])


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


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        self._p_description = "Detailed Description."








@register("Namespace",root=True)
class Namespace(Base):
    """
    This is the namespace block class. It implements the Socrates' Reference abstract block class.
    It represents a C++ namespace. It is the root block type of a C++ project.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new namespace block.
        """
        Base.__init__(self)
        self._p_hidden = "0"


    ####################
    # PUBLIC - Methods #
    ####################


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Namespace","Enumeration","Variable","Function","Class","Union")


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.clearProperties(self)
        self._p_hidden = "0"


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = Base.editDefinitions(self)
        ret.append(ut.checkboxEdit("Hidden","_p_hidden"))
        return ret


    def hasFunctions(self):
        """
        Getter method.

        return : True if this namespace has one or more functions or false otherwise.
        """
        for child in self:
            if child._TYPE_ == "Function":
                return True
        return False


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        if self.isHidden():
            return qtg.QIcon(":/cpp/hidden_namespace.svg")
        else:
            return qtg.QIcon(":/cpp/namespace.svg")


    def isHidden(self):
        """
        Getter method.

        return : True if this is a hidden namespace that must not have its name added to the scoping
                 of source code files nor have its own header or source file. If this is a regular
                 visible namespace then false is returned.
        """
        return bool(int(self._p_hidden))


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
        Base.setDefaultProperties(self)
        self._p_name = "namespace"
        self._p_hidden = "0"
