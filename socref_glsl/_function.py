"""
Contains the Function class.
"""
import html
from PySide2 import QtGui as qtg
from socref import edit
from socref import register
from . import settings
from ._base import Base








@register("Function")
class Function(Base):
    """
    This is the function block class. It implements the Socrates' Reference abstract block class. It
    represents a GLSL function.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new function block.
        """
        Base.__init__(self)
        self._p_returnType = ""
        self._p_returnDescription = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def build(self, definition, begin=""):
        """
        Implements the socref_glsl.block.Base interface.

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
        ret = [""]*settings.H1LINES
        ret += self.__buildComments_(begin)
        ret += self.__buildDeclaration_(begin)
        ret += definition["functions"].pop(self._p_name,["{"]) + ["}"]
        return ret


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return ("Variable",)


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.clearProperties(self)
        self._p_returnType = ""
        self._p_returnDescription = ""


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return self._p_name + "(%s)"%str(len(self))


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return_ = ""
        if self._p_returnType != "void":
            return_ = (
                "<p><b>%s</b> : %s</p>"
                % (html.escape(self._p_returnType),html.escape(self._p_returnDescription))
            )
        return_ = edit.richText(2,"Return",return_)
        return Base.displayView(self)+self.__argumentsView_()+return_


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = Base.editDefinitions(self)
        ret.append(edit.lineEdit("Return Type:","_p_returnType"))
        ret.append(edit.textEdit("Return Description:","_p_returnDescription",speller=True))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return qtg.QIcon(":/glsl/function.svg")


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.setDefaultProperties(self)
        self._p_name = "function"
        self._p_returnType = "void"
        self._p_returnDescription = ""


    #####################
    # PRIVATE - Methods #
    #####################


    def __argumentsView_(self):
        """
        Getter method.

        Returns
        -------
        ret0 : rich text
               Detailed view of all this function's arguments. If this function has no arguments
               then this returns an empty string.
        """
        return edit.richText(
            2
            ,"Arguments"
            ,"".join((child.argumentView() for child in self if child._TYPE_ == "Variable"))
        )


    def __buildComments_(self, begin):
        """
        Getter method.

        Parameters
        ----------
        begin : string
                Added to the beginning of every returned line of code.

        Returns
        -------
        ret0 : list
               Source code lines that is the block comment for this function.
        """
        ret = [begin+"/*!"]
        commentBlank = begin+" *"
        commentBegin = commentBlank+" "
        ret += edit.wrapText(self._p_description,commentBegin,columns=settings.COLUMNS)
        for arg in self:
            ret += [commentBlank] + arg.buildComment(commentBegin)
        if self._p_returnDescription:
            initial = "@return : "
            ret += (
                [commentBlank]
                + edit.wrapText(
                    initial+self._p_returnDescription
                    ,commentBegin
                    ," "*len(initial)
                    ,settings.COLUMNS
                )
            )
        ret.append(begin+" */")
        return ret


    def __buildDeclaration_(self, begin):
        """
        Getter method.

        Parameters
        ----------
        begin : string
                Added to the beginning of every returned line of code.

        Returns
        -------
        ret0 : list
               Lines that is the declaration, or header, of this function. If this function has no
               arguments then a single line is returned.
        """
        ret = []
        line = begin+self._p_returnType+" "+self._p_name+"("
        args = []
        for child in self:
            if child._TYPE_ == "Variable":
                args.append(child.buildArgument())
        if not args:
            line += ")"
            ret.append(line)
        else:
            newBegin = begin + " "*settings.INDENT
            ret.append(line)
            first = True
            for arg in args:
                ret.append(newBegin+arg)
                if first:
                    newBegin = newBegin + ","
                    first = False
            ret.append(begin+")")
        return ret
