"""
Contains the Object class.
"""
import html
from PySide2 import QtGui as qtg
from socref import public as scr
from ._package import Package
from . import settings








@scr.register("Object")
class Object(Package):
    """
    This is the object block class. It implements the Socrates' Reference
    abstract block class. It represents a python object.
    """


    def __init__(
        self
        ):
        """
        Initializes a new object block.
        """
        super().__init__()
        self._p_assignment = ""


    def argumentView(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : rich text
               Paragraph that describes this object as an argument of a
               function.
        """
        ret = "<p><b>%s " % html.escape(self._p_name)
        if self._p_assignment:
            ret += " =</b> " + html.escape(self._p_assignment) + " : "
        else:
            ret += "</b> : "
        ret += html.escape(self._p_description) + "</p>"
        return ret


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
        ret = [""]*settings.H3LINES
        blankCommentLine = begin + "#"
        ret.append(blankCommentLine)
        ret += scr.wrapBlocks(self._p_description,begin=begin + "# ",columns=settings.COLUMNS)
        ret.append(blankCommentLine)
        line = begin + self._p_name
        if self._p_assignment:
            line += " = " + self._p_assignment
        ret.append(line)
        return ret


    def buildArgument(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               The source code for this object as a function argument.
        """
        ret = self._p_name
        if self._p_assignment:
            ret += "=" + self._p_assignment
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
                The indent that is appended to every line of returned source
                code.

        Returns
        -------
        ret0 : list
               The source code lines doc string fragment for this object as a
               function argument.
        """
        return Object.comment(self._p_name,self._p_description,begin)


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
        Package.clearProperties(self)
        self._p_assignment = ""


    @staticmethod
    def comment(
        name
        ,description
        ,begin
        ):
        """
        Getter method.

        Parameters
        ----------
        name : string
               The name of the variable whose doc string is returned.
        description : string
                      Description of the variable separated by at least two
                      lines. The first line is its type and all other lines is
                      the actual description.
        begin : string
                The indent that is appended to every line of returned source
                code.

        Returns
        -------
        ret0 : list
               A doc string lines fragment for the given variable name and
               description as a function argument or return value.
        """
        parts = description.split("\n")
        if not parts:
            parts = ["object",""]
        elif len(parts) == 1:
            parts = ["object",parts[0]]
        elif len(parts) > 2:
            parts = parts[:1]+[" ".join(parts[1:])]
        initial = name + " : "
        ret = scr.wrapText(
            initial+parts[0]
            ,begin=begin
            ,after=" "*len(initial)
            ,columns=settings.COLUMNS
        )
        ret += scr.wrapText(parts[1],begin=begin + " "*len(initial),columns=settings.COLUMNS)
        return ret


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
        ret = self._p_name
        if self.isArgument() and self._p_assignment:
            ret += " ="
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
        assignment = scr.richText(2,"Assignment",html.escape(self._p_assignment))
        return Package.displayView(self) + assignment


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
        ret = Package.editDefinitions(self)
        ret.append(scr.lineEdit("Assignment:","_p_assignment"))
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
        if self.isArgument():
            return qtg.QIcon(":/python/object.svg")
        elif self._p_name.startswith("__"):
            return qtg.QIcon(":/python/private_member.svg")
        elif self._p_name.startswith("_"):
            return qtg.QIcon(":/python/protected_member.svg")
        else:
            return qtg.QIcon(":/python/public_member.svg")


    def inClass(
        self
        ):
        """
        This interface is a getter method.

        Returns
        -------
        ret0 : bool
               True if this object is part of a class or false otherwise.
        """
        return self.parent()._TYPE_ == "Class" or self.parent()._TYPE_ == "Access"


    def isArgument(
        self
        ):
        """
        This interface is a getter method.

        Returns
        -------
        ret0 : bool
               True if this object is an argument of a function or false
               otherwise.
        """
        return not self.parent() or self.parent()._TYPE_ == "Function"


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
        Package.setDefaultProperties(self)
        self._p_name = "object"
        self._p_assignment = ""
