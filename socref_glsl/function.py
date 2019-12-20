"""
Contains the function block class.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import settings
from . import shader








@register("Function")
class Function(shader.Base):
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
        shader.Base.__init__(self)
        self._p_returnType = ""
        self._p_returnDescription = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def build(self, definition, begin=""):
        """
        Implements the .shader.Base interface.

        definition : See interface docs.

        begin : See interface docs.

        return : See interface docs.
        """
        ret = self.__buildComments_(begin)
        ret += self.__buildDeclaration_(begin)
        ret += ["{","}"]
        return ret


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Variable",)


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        shader.Base.clearProperties(self)
        self._p_returnType = ""
        self._p_returnDescription = ""


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return self._p_name + "(%s)"%str(len(self))


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return_ = ""
        if self._p_returnType != "void":
            return_ = (
                "<p><b>%s</b> : %s</p>"
                % (html.escape(self._p_returnType),html.escape(self._p_returnDescription))
            )
        return_ = ut.richText(2,"Return",return_)
        return shader.Base.displayView(self)+self.__argumentsView_()+return_


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = shader.Base.editDefinitions(self)
        ret.append(ut.lineEdit("Return Type:","_p_returnType"))
        ret.append(ut.textEdit("Return Description:","_p_returnDescription",speller=True))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/glsl/function.svg")


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        shader.Base.setDefaultProperties(self)
        self._p_name = "function"
        self._p_returnType = "void"
        self._p_returnDescription = ""


    #####################
    # PRIVATE - Methods #
    #####################


    def __argumentsView_(self):
        """
        Getter method.

        return : Rich text detailed view of all this function's arguments. If this function has no
                 arguments then this returns an empty string.
        """
        return ut.richText(
            2
            ,"Arguments"
            ,"".join((child.argumentView() for child in self if child._TYPE_ == "Variable"))
        )


    def __buildComments_(self, begin):
        """
        Getter method.

        begin : A string that is added to the beginning of every returned line of code.

        return : A list of source code lines that is the block comment for this function.
        """
        ret = [""]*settings.H2LINES
        ret.append(begin+"/*!")
        commentBlank = begin+" *"
        commentBegin = commentBlank+" "
        ret += ut.wrapText(self._p_description,commentBegin,columns=settings.COLUMNS)
        for arg in self:
            ret += [commentBlank] + arg.buildComment(commentBegin)
        if self._p_returnDescription:
            initial = "@return : "
            ret += (
                [commentBlank]
                + ut.wrapText(
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

        begin : A string that is added to the beginning of every returned line of returned code.

        return : A list of lines that is the declaration, or header, of this function. If this
                 function has no arguments then a single line is returned.
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
