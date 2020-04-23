"""
Contains the Class class.
"""
from PySide2 import QtGui as qtg
from socref import edit
from socref import register
from ._descriptor import Descriptor
from . import block
from . import settings








@register("Class")
class Class(Descriptor):
    """
    This is the class block class. It implements the Socrates' Reference abstract block class. It
    represents a python class.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(
        self
        ):
        """
        Initializes a new class block.
        """
        Descriptor.__init__(self)
        self._p_parents = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def build(
        self
        ,definition
        ,begin=""
        ):
        """
        Implements the socref_python.block.Package interface.

        Parameters
        ----------
        definition : object
                     See interface docs.
        begin : object
                See interface docs.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = []
        ret.append('"""')
        ret += edit.wrapBlocks("Contains the %s class."%(self._p_name,),columns=settings.COLUMNS)
        ret.append('"""')
        ret += definition.pop("header")
        script = definition.pop("script")
        definition = definition["classes"].get(self._p_name,{"functions": {}})
        ret += [""]*settings.H1LINES
        ret += self._buildDescriptors_(begin)
        ret.append("%sclass %s(%s):" % (begin,self._p_name,self.__buildParents_()))
        ret.append(begin + " "*settings.INDENT + '"""')
        ret += edit.wrapBlocks(
            self._p_description
            ,begin=begin + " "*settings.INDENT
            ,columns=settings.COLUMNS
        )
        ret.append(begin + " "*settings.INDENT + '"""')
        (regular,classes) = self._buildChildren_(definition,begin + " " * settings.INDENT)
        ret += regular
        if script:
            ret += [""]*settings.H1LINES + [settings.SCRIPT_HEADER] + script
        return ret


    def buildList(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return ("Access",)


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Descriptor.clearProperties(self)
        self._p_parents = ""


    def displayName(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return self._p_name + self._descriptorsName_()


    def displayView(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        parents = edit.richText(
            2
            ,"Parents"
            ,"".join(("<li>%s</li>" % parent for parent in self._p_parents.split("\n") if parent))
        )
        return block.Package.displayView(self) + parents + self._descriptorsView_()


    def editDefinitions(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = block.Package.editDefinitions(self)
        ret.append(edit.textEdit("Parents:","_p_parents"))
        ret.append(self._descriptorsEditDefinition_())
        return ret


    def icon(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        if self.isAbstract():
            return qtg.QIcon(":/python/abstract_class.svg")
        else:
            return qtg.QIcon(":/python/class.svg")


    def isAbstract(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this class contains any abstract functions or false otherwise.
        """
        for access in self:
            if access.hasAbstract():
                return True
        return False


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Descriptor.setDefaultProperties(self)
        self._p_name = "class"
        self._p_parents = ""


    #####################
    # PRIVATE - Methods #
    #####################


    def __buildParents_(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               The source code fragment of this class block's parents used in its source code header
               line.
        """
        return ", ".join((parent for parent in self._p_parents.split("\n") if parent))
