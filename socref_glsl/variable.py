"""
Contains the variable block class.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import settings
from . import shader








@register("Variable")
class Variable(shader.Base):
    """
    This is the variable block class. It implements the Socrates' Reference abstract block class. It
    represents a GLSL variable.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new variable block.
        """
        shader.Base.__init__(self)
        self._p_type = ""
        self._p_assignment = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def argumentView(self):
        """
        Getter method.

        Returns
        -------
        ret0 : rich text
               Paragraph that describes this variable as an argument.
        """
        ret = "<p><b>%s "%html.escape(self._p_type.replace("@",self._p_name))
        if self._p_assignment:
            ret += " =</b> "+html.escape(self._p_assignment)+" : "
        else:
            ret += "</b> : "
        ret += html.escape(self._p_description)+"</p>"
        return ret


    def build(self, definition, begin=""):
        """
        Implements the .shader.Base interface.

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
        if self.inStructure():
            ret = ut.wrapText(self._p_description,begin+"/// ",columns=settings.COLUMNS)
        else:
            ret = [""]*settings.H2LINES
            ret.append(begin+"/*!")
            ret += ut.wrapText(self._p_description,begin+" * ",columns=settings.COLUMNS)
            ret.append(begin+" */")
        ret.append(begin+self._p_type.replace("@",self._p_name)+";")
        return ret


    def buildArgument(self):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               A source code fragment that is the argument of this variable.
        """
        return self._p_type.replace("@",self._p_name)


    def buildComment(self, begin):
        """
        Getter method.

        Parameters
        ----------
        begin : string
                Added to the beginning of returned lines of comment code.

        Returns
        -------
        ret0 : list
               Source code lines that is a comment fragment for this variable as an argument. This
               returns the correct doxygen syntax.
        """
        header = "@param %s : " % self._p_name
        return ut.wrapText(
            header + self._p_description
            ,begin
            ,begin + " "*len(header)
            ,settings.COLUMNS
        )


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return ()


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        shader.Base.clearProperties(self)
        self._p_type = ""
        self._p_assignment = ""


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        if self._p_name:
            return self._p_name
        else:
            return self._p_type.split()[-1]


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        type_ = "<h2>Type</h2><p>"+html.escape(self._p_type)+"</p>"
        assignment = ut.richText(2,"Assignment",html.escape(self._p_assignment))
        return shader.Base.displayView(self)+type_+assignment


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = shader.Base.editDefinitions(self)
        ret.append(ut.lineEdit("Type:","_p_type"))
        ret.append(ut.lineEdit("Assignment:","_p_assignment"))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return qtg.QIcon(":/glsl/variable.svg")


    def inStructure(self):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this variable is part of a structure or false otherwise.
        """
        return self.parent()._TYPE_ == "Structure"


    def isArgument(self):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this variable is an argument of a function or false otherwise.
        """
        return self.parent()._TYPE_ == "Function"


    def isVolatileAbove(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return self.isArgument()


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        shader.Base.setDefaultProperties(self)
        self._p_name = "variable"
        self._p_type = "float @"
