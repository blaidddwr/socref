"""
Contains the Template class.
"""
import html
from PySide2 import QtGui as qtg
from socref import edit
from socref import register
from . import settings
from ._base import Base








@register("Template")
class Template(Base):
    """
    This is the template block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ template.
    """


    def __init__(
        self
        ):
        """
        Initializes a new namespace block.
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
               Paragraph that describes this template as an argument.
        """
        ret = "<p><b>%s "%html.escape(self._p_type.replace("@",self._p_name))
        if self._p_assignment:
            ret += " =</b> "+html.escape(self._p_assignment)+" : "
        else:
            ret += "</b> : "
        ret += html.escape(self._p_description)+"</p>"
        return ret


    def buildArgument(
        self
        ,isDeclaration
        ):
        """
        Getter method.

        Parameters
        ----------
        isDeclaration : bool
                        True if this is returned for a function declaration or
                        false if it is for a function definition.

        Returns
        -------
        ret0 : string
               Source code fragment that is the argument of this template,
               including any initialization if this is for a declaration.
        """
        ret = self._p_type.replace("@",self._p_name)
        if self._p_assignment and isDeclaration:
            ret += "="+self._p_assignment
        return ret


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
               Source code lines that is a comment fragment for this template as
               an argument. This returns the correct doxygen syntax based off
               this block being an actual template or variable argument.
        """
        header = ""
        if self._TYPE_ == "Template":
            header = "@tparam %s : " % self._p_name
        else:
            header = "@param %s : " % self._p_name
        ret = edit.wrapText(
            header + self._p_description
            ,begin
            ," "*len(header)
            ,settings.COLUMNS
        )
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
        return ()


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.clearProperties(self)
        self._p_name = ""
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
        return self._p_name


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
        type_ = edit.richText(2,"Type",html.escape(self._p_type))
        assignment = edit.richText(2,"Assignment",html.escape(self._p_assignment))
        return Base.displayView(self) + type_ + assignment


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
        return qtg.QIcon(":/cpp/template.svg")


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
        return True


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.setDefaultProperties(self)
        self._p_name = "Template"
        self._p_type = "class @"
        self._p_assignment = ""
