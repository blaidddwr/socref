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
class Class(function.Descriptor):
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


    def build(self, definition, begin=""):
        """
        Implements the .package.Package interface.

        definition : See interface docs.

        begin : See interface docs.

        return : See interface docs.
        """
        #
        # Get the scanned dictionary definition of this class.
        #
        definition = definition["classes"].get(self._p_name,{"functions":{}})
        ret = [""]*settings.H1LINES
        ret += self._buildDescriptors_(begin)
        ret.append("%sclass %s(%s):" % (begin,self._p_name,self.__buildParents_()))
        ret.append(begin + " "*settings.INDENT + '"""')
        ret += ut.wrapBlocks(
            self._p_description
            ,begin=begin + " "*settings.INDENT
            ,columns=settings.COLUMNS
        )
        ret.append(begin + " "*settings.INDENT + '"""')
        ret += self._buildChildren_(definition,begin + " " * settings.INDENT)
        return ret


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Access",)


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        function.Descriptor.clearProperties(self)
        self._p_parents = ""


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return self._p_name + self._descriptorsName_()


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        parents = ut.richText(
            2
            ,"Parents"
            ,"".join(("<li>%s</li>" % parent for parent in self._p_parents.split("\n") if parent))
        )
        return package.Package.displayView(self) + parents + self._descriptorsView_()


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = package.Package.editDefinitions(self)
        ret.append(ut.textEdit("Parents:","_p_parents"))
        ret.append(self._descriptorsEditDefinition_())
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        if self.isAbstract():
            return qtg.QIcon(":/python/abstract_class.svg")
        else:
            return qtg.QIcon(":/python/class.svg")


    def isAbstract(self):
        """
        Getter method.

        return : True if this class contains any abstract functions or false otherwise.
        """
        for access in self:
            if access.hasAbstract():
                return True
        return False


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        function.Descriptor.setDefaultProperties(self)
        self._p_name = "class"
        self._p_parents = ""


    #####################
    # PRIVATE - Methods #
    #####################


    def __buildParents_(self):
        """
        Getter method.

        return : A string that is the source code fragment of this class block's parents used in its
                 source code header line.
        """
        return ", ".join((parent for parent in self._p_parents.split("\n") if parent))
