"""
Contains the function block definition and generic descriptor block.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from socref import abstract
from . import settings
from . import package








class Descriptor(package.Package):
    """
    This is the descriptor class. It partially implements the Socrates' Reference abstract block
    class. This provides a descriptors property along with utility methods for it. This is meant to
    act as a base class for any block that has descriptors.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new descriptor block.
        """
        package.Package.__init__(self)
        self._p_descriptors = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        package.Package.clearProperties(self)
        self._p_descriptors = ""


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        package.Package.setDefaultProperties(self)
        self._p_descriptors = ""


    #######################
    # PROTECTED - Methods #
    #######################


    def _buildDescriptors_(self, begin):
        """
        Getter method.

        begin : The indent string that is added to the beginning of every line of returned code.

        return : A string that is the source code for this block's descriptors. If this block has no
                 descriptors an empty string is returned.
        """
        return [begin + "@" + line for line in self._p_descriptors.split("\n") if line]


    def _descriptorsEditDefinition_(self):
        """
        Getter Method.

        return : The edit definition for this block's descriptors property.
        """
        return ut.textEdit("Descriptors:","_p_descriptors")


    def _descriptorsName_(self):
        """
        Getter method.

        return : A string that is a decoration for a block's display name providing information
                 about its descriptors. If this block has no descriptors then an empty string is
                 returned.
        """
        if self._p_descriptors:
            return " @"
        else:
            return ""


    def _descriptorsView_(self):
        """
        Getter method.

        return : A rich text string providing detailed information about this block's descriptors.
                 If this block has no descriptors then an empty string is returned.
        """
        return ut.richText(
            2
            ,"Descriptors"
            ,"".join(
                ("<p>@%s</p>" % html.escape(line)
                 for line in self._p_descriptors.split("\n") if line)
             )
        )








@register("Function")
class Function(Descriptor):
    """
    This is the function block class. It implements the Socrates' Reference abstract block class. It
    represents a python function.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new function block.
        """
        Descriptor.__init__(self)
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_static = "0"
        self._p_abstract = "0"


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
        ret = []
        if self.isMethod():
            ret += [""]*settings.H3LINES
        else:
            ret += [""]*settings.H2LINES
        ret += self.__buildHeader_(begin)
        ret += self.__buildDocString_(begin)
        ret += self.__buildLines_(definition["functions"].pop(self._p_name,[]),begin)
        return ret


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Object",)


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Descriptor.clearProperties(self)
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_static = "0"
        self._p_abstract = "0"


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = ""
        ret += "%s(%i)%s" % (self._p_name,len(self),self._descriptorsName_())
        if self._p_return_description:
            ret += " ..."
        return ret


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        self.__checkFlags_()
        return_ = ut.richText(2,"Return",html.escape(self._p_return_description))
        return (
            package.Package.displayView(self)
            + self.__argumentsView_()
            + return_
            + self.__flagsView_()
            + self._descriptorsView_()
        )


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = package.Package.editDefinitions(self)
        ret.append(ut.textEdit("Return:","_p_return_description",speller=True))
        if self.isMethod():
            ret.append(ut.checkboxEdit("Static","_p_static"))
            ret.append(ut.checkboxEdit("Abstract","_p_abstract"))
        else:
            ret.append(ut.hiddenEdit("_p_static","0"))
            ret.append(ut.hiddenEdit("_p_abstract","0"))
        ret.append(ut.textEdit("Inline Comments:","_p_inlines",speller=True))
        ret.append(self._descriptorsEditDefinition_())
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        if self._p_name.startswith("__") and self._p_name.endswith("__"):
            return qtg.QIcon(":/python/operator.svg")
        elif self.isStatic():
            return qtg.QIcon(":/python/static_function.svg")
        elif self.isAbstract():
            return qtg.QIcon(":/python/abstract_function.svg")
        else:
            return qtg.QIcon(":/python/function.svg")


    def isAbstract(self):
        """
        Getter method.

        return : True if this function is abstract or false otherwise.
        """
        return bool(int(self._p_abstract))


    def isMethod(self):
        """
        Getter method.

        return : True if this function is a method of a class or false otherwise.
        """
        return self.parent()._TYPE_ == "Access"


    def isStatic(self):
        """
        Getter method.

        return : True if this function is static or false otherwise.
        """
        return bool(int(self._p_static))


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Descriptor.setDefaultProperties(self)
        self._p_name = "function"
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_static = "0"
        self._p_abstract = "0"


    #####################
    # PRIVATE - Methods #
    #####################


    def __argumentsView_(self):
        """
        Getter method.

        return : Rich text detailed view of all this function's arguments. If this function has no
                 arguments then this returns an empty string.
        """
        return ut.richText(2,"Arguments","".join((arg.argumentView() for arg in self)))


    def __buildDocString_(self, begin):
        """
        Getter method.

        begin : The indent string that is added to the beginning of every line of returned code.

        return : A list of lines that is the source code doc string of this function.
        """
        ret = [begin + " "*settings.INDENT + '"""']
        ret += ut.wrapBlocks(
            self._p_description
            ,begin=begin + " "*settings.INDENT
            ,columns=settings.COLUMNS
        )
        for arg in self:
            ret += [""] + arg.buildComment(begin + " "*settings.INDENT)
        if self._p_return_description:
            initial = "return : "
            ret += (
                [""]
                + ut.wrapText(
                    initial + self._p_return_description
                    ,begin=begin + " "*settings.INDENT
                    ,after=" "*len(initial)
                    ,columns=settings.COLUMNS
                )
            )
        ret.append(ret[0])
        return ret


    def __buildHeader_(self, begin):
        """
        Getter method.

        begin : The indent string that is added to the beginning of every line of returned code.

        return : A list of lines that is the source code of this function's header. The header
                 includes any descriptors and the define line.
        """
        ret = self._buildDescriptors_(begin)
        if self.isAbstract():
            ret.append(begin + "@" + settings.ABSTRACT_DESCRIPTOR)
        arguments = [arg.buildArgument() for arg in self]
        if self.isMethod() and not self.isStatic():
            arguments.insert(0,"self")
        ret.append("%sdef %s(%s):" % (begin,self._p_name,", ".join(arguments)))
        return ret


    def __buildLines_(self, lines, begin):
        """
        Getter method.

        lines : A list of code lines used to generate the returned source code lines.

        begin : The indent string that is added to the beginning of every line of returned code.

        return : A list of source code lines generated from the given list of code lines and this
                 function's inline comments.
        """
        if not lines:
            return [begin + " "*settings.INDENT + "pass"]
        ret = []
        inlines = [line for line in self._p_inlines.split("\n\n") if line]
        for line in lines:
            if line.endswith("#"):
                if inlines:
                    ret.append(begin + " "*settings.INDENT + line)
                    ret += ut.wrapText(
                        inlines.pop(0)
                        ,begin=begin + " "*settings.INDENT + line + " "
                        ,columns=settings.COLUMNS
                    )
                    ret.append(begin + " "*settings.INDENT + line)
                else:
                    ret.append(begin + " "*settings.INDENT + line)
            else:
                ret.append(begin + " "*settings.INDENT + line)
        return ret


    def __checkFlags_(self):
        """
        Sets this function's flags to legal values if it is not a method.
        """
        if not self.isMethod():
            self._p_static = "0"
            self._p_abstract = "0"


    def __flagsView_(self):
        """
        Getter method.

        return : Rich text list of flags this block has enabled. If this block has no flags enabled
                 then an empty string is returned.
        """
        flags = []
        if self.isStatic():
            flags.append("Static")
        if self.isAbstract():
            flags.append("Abstract")
        return ut.richTextList(2,"Flags",flags)
