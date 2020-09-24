"""
Contains the Variable class.
"""
import html
from PySide2 import QtGui as qtg
from socref import edit
from socref import register
from . import settings
from ._base import Base








@register("Variable")
class Variable(Base):
    """
    This is the variable block class. It implements the Socrates' Reference
    abstract block class. It represents a GLSL variable.
    """


    def __init__(
        self
        ):
        """
        Initializes a new variable block.
        """
        super().__init__()
        self._p_type = ""
        self._p_assignment = ""


    def argumentView(
        self
        ):
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


    def build(
        self
        ,definition
        ,begin=""
        ):
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
        ret = []
        if self.inStructure():
            ret = edit.wrapText(self._p_description,begin+"/// ",columns=settings.COLUMNS)
        else:
            ret = [""]*settings.H2LINES
            ret.append(begin+"/*!")
            ret += edit.wrapText(self._p_description,begin+" * ",columns=settings.COLUMNS)
            ret.append(begin+" */")
        ret.append(begin+self._p_type.replace("@",self._p_name)+";")
        return ret


    def buildArgument(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               A source code fragment that is the argument of this variable.
        """
        return self._p_type.replace("@",self._p_name)


    def buildComment(
        self
        ,begin
        ):
        """
        Getter method.

        Parameters
        ----------
        begin : string
                Added to the beginning of returned lines of comment code.

        Returns
        -------
        ret0 : list
               Source code lines that is a comment fragment for this variable as
               an argument. This returns the correct doxygen syntax.
        """
        header = "@param %s : " % self._p_name
        return edit.wrapText(
            header + self._p_description
            ,begin
            ,begin + " "*len(header)
            ,settings.COLUMNS
        )


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
        return ()


    def buildSignature(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               The signature of this variable as an argument.
        """
        return "_".join(self._p_type.replace("@","").split())


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.clearProperties(self)
        self._p_type = ""
        self._p_assignment = ""


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
        if self._p_name:
            return self._p_name
        else:
            return self._p_type.split()[-1]


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
        type_ = "<h2>Type</h2><p>"+html.escape(self._p_type)+"</p>"
        assignment = edit.richText(2,"Assignment",html.escape(self._p_assignment))
        return Base.displayView(self)+type_+assignment


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
        ret = Base.editDefinitions(self)
        ret.append(edit.lineEdit("Type:","_p_type"))
        ret.append(edit.lineEdit("Assignment:","_p_assignment"))
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
        return qtg.QIcon(":/glsl/variable.svg")


    def inStructure(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this variable is part of a structure or false otherwise.
        """
        return self.parent()._TYPE_ == "Structure"


    def isArgument(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this variable is an argument of a function or false
               otherwise.
        """
        return self.parent()._TYPE_ == "Function"


    def isVolatileAbove(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return self.isArgument()


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.setDefaultProperties(self)
        self._p_name = "variable"
        self._p_type = "float @"
