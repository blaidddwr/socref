"""
Contains the Function class.
"""
import html
from PySide2 import QtGui as qtg
from socref import edit
from socref import register
from ._descriptor import Descriptor
from . import block
from . import settings








@register("Function")
class Function(Descriptor):
    """
    This is the function block class. It implements the Socrates' Reference
    abstract block class. It represents a python function.
    """


    def __init__(
        self
        ):
        """
        Initializes a new function block.
        """
        Descriptor.__init__(self)
        self._p_returnDescription = ""
        self._p_static = "0"
        self._p_abstract = "0"


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
        if self.isMethod():
            ret += [""]*settings.H3LINES
        else:
            ret += [""]*settings.H2LINES
        ret += self.__buildHeader_(begin)
        ret += self.__buildDocString_(begin)
        ret += self.__buildLines_(definition["functions"].pop(self._p_name,[]),begin)
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
        return ("Object",)


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Descriptor.clearProperties(self)
        self._p_returnDescription = ""
        self._p_static = "0"
        self._p_abstract = "0"


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
        ret = ""
        ret += "%s(%i)%s" % (self._p_name,len(self),self._descriptorsName_())
        if self._p_returnDescription:
            ret += " ..."
        return ret


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
        self.__checkFlags_()
        return_ = edit.richText(2,"Return",html.escape(self._p_returnDescription))
        return (
            block.Package.displayView(self)
            + self.__argumentsView_()
            + return_
            + self.__flagsView_()
            + self._descriptorsView_()
        )


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
        ret.append(edit.textEdit("Return:","_p_returnDescription",speller=True))
        if self.isMethod():
            ret.append(edit.checkboxEdit("Static","_p_static"))
            ret.append(edit.checkboxEdit("Abstract","_p_abstract"))
        else:
            ret.append(edit.hiddenEdit("_p_static","0"))
            ret.append(edit.hiddenEdit("_p_abstract","0"))
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
        if not self.isMethod():
            return qtg.QIcon(":/python/function.svg")
        elif self._p_name.startswith("__") and self._p_name.endswith("__"):
            return qtg.QIcon(":/python/operator.svg")
        elif self._p_name.startswith("__"):
            if self.isStatic():
                return qtg.QIcon(":/python/static_private_method.svg")
            elif self.isAbstract():
                return qtg.QIcon(":/python/abstract_private_method.svg")
            else:
                return qtg.QIcon(":/python/private_method.svg")
        elif self._p_name.startswith("_"):
            if self.isStatic():
                return qtg.QIcon(":/python/static_protected_method.svg")
            elif self.isAbstract():
                return qtg.QIcon(":/python/abstract_protected_method.svg")
            else:
                return qtg.QIcon(":/python/protected_method.svg")
        else:
            if self.isStatic():
                return qtg.QIcon(":/python/static_public_method.svg")
            elif self.isAbstract():
                return qtg.QIcon(":/python/abstract_public_method.svg")
            else:
                return qtg.QIcon(":/python/public_method.svg")


    def isAbstract(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this function is abstract or false otherwise.
        """
        return bool(int(self._p_abstract))


    def isMethod(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this function is a method of a class or false otherwise.
        """
        return (
            self.parent()
            and ( self.parent()._TYPE_ == "Class" or self.parent()._TYPE_ == "Access" )
        )


    def isStatic(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this function is static or false otherwise.
        """
        return bool(int(self._p_static))


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Descriptor.setDefaultProperties(self)
        self._p_name = "function"
        self._p_returnDescription = ""
        self._p_static = "0"
        self._p_abstract = "0"


    def __argumentsView_(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : rich text
               Detailed view of all this function's arguments. If this function
               has no arguments then this returns an empty string.
        """
        return edit.richText(2,"Arguments","".join((arg.argumentView() for arg in self)))


    def __buildDocString_(
        self
        ,begin
        ):
        """
        Getter method.

        Parameters
        ----------
        begin : string
                The indent that is added to the beginning of every line of
                returned code.

        Returns
        -------
        ret0 : list
               The source code doc string lines of this function.
        """
        newBegin = begin + " "*settings.INDENT
        ret = [newBegin+'"""']
        ret += edit.wrapBlocks(
            self._p_description
            ,begin=newBegin
            ,columns=settings.COLUMNS
        )
        args = []
        for arg in self:
            args += arg.buildComment(newBegin)
        if args:
            ret += ["",newBegin+"Parameters",newBegin+"----------"]
            ret += args
        returns = [desc for desc in self._p_returnDescription.split("\n\n") if desc]
        if returns:
            ret += ["",newBegin+"Returns",newBegin+"-------"]
            i = 0
            for desc in returns:
                ret += block.Object.comment("ret"+str(i),desc,newBegin)
                i += 1
        ret.append(ret[0])
        return ret


    def __buildHeader_(
        self
        ,begin
        ):
        """
        Getter method.

        Parameters
        ----------
        begin : string
                The indent that is added to the beginning of every line of
                returned code.

        Returns
        -------
        ret0 : list
               The source code lines of this function's header. The header
               includes any descriptor and the define lines.
        """
        ret = self._buildDescriptors_(begin)
        if self.isAbstract():
            ret.append(begin + "@" + settings.ABSTRACT_DESCRIPTOR)
        args = [arg.buildArgument() for arg in self]
        if self.isMethod() and not self.isStatic():
            args.insert(0,"self")
        if not args:
            ret.append("%sdef %s():"%(begin,self._p_name))
        else:
            ret.append("%sdef %s("%(begin,self._p_name))
            newBegin = begin + " "*settings.INDENT
            end = newBegin+"):"
            first = True
            for arg in args:
                ret.append(newBegin+arg)
                if first:
                    newBegin += ","
                    first = False
            ret.append(end)
        return ret


    def __buildLines_(
        self
        ,lines
        ,begin
        ):
        """
        Getter method.

        Parameters
        ----------
        lines : list
                A list of code lines used to generate the returned source code
                lines.
        begin : string
                The indent that is added to the beginning of every line of
                returned code.

        Returns
        -------
        ret0 : list
               Source code lines generated from the given list of code lines.
        """
        if not lines:
            return [begin + " "*settings.INDENT + "pass"]
        else:
            newBegin = begin + " "*settings.INDENT
            return [newBegin+line for line in lines if line]


    def __checkFlags_(
        self
        ):
        """
        Sets this function's flags to legal values if it is not a method.
        """
        if not self.isMethod():
            self._p_static = "0"
            self._p_abstract = "0"


    def __flagsView_(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : list
               Rich text list of flags this block has enabled. If this block has
               no flags enabled then an empty string is returned.
        """
        flags = []
        if self.isStatic():
            flags.append("Static")
        if self.isAbstract():
            flags.append("Abstract")
        return edit.richTextList(2,"Flags",flags)
