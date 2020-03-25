"""
Contains the Object class.
"""
import html
from PySide2 import QtGui as qtg
from socref import edit
from socref import register
from ._package import Package
from . import settings








@register("Object")
class Object(Package):
    """
    This is the object block class. It implements the Socrates' Reference abstract block class. It
    represents a python object.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new object block.
        """
        Package.__init__(self)
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
               Paragraph that describes this object as an argument of a function.
        """
        ret = "<p><b>%s " % html.escape(self._p_name)
        if self._p_assignment:
            ret += " =</b> " + html.escape(self._p_assignment) + " : "
        else:
            ret += "</b> : "
        ret += html.escape(self._p_description) + "</p>"
        return ret


    def build(self, definition, begin=""):
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
        ret += edit.wrapBlocks(self._p_description,begin=begin + "# ",columns=settings.COLUMNS)
        ret.append(blankCommentLine)
        line = begin + self._p_name
        if self._p_assignment:
            line += " = " + self._p_assignment
        ret.append(line)
        return ret


    def buildArgument(self):
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


    def buildComment(self, begin):
        """
        Getter method.

        Parameters
        ----------
        begin : string
                The indent that is appended to every line of returned source code.

        Returns
        -------
        ret0 : list
               The source code lines doc string fragment for this object as a function argument.
        """
        desc = self._p_description.split("\n")
        if len(desc) >= 2:
            initial = self._p_name + " : "
            ret = edit.wrapText(
                initial+desc[0]
                ,begin=begin
                ,after=" "*len(initial)
                ,columns=settings.COLUMNS
            )
            ret += edit.wrapText(desc[1],begin=begin + " "*len(initial),columns=settings.COLUMNS)
            return ret
        else:
            return [begin+"UNKNOWN"]


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
        Package.clearProperties(self)
        self._p_assignment = ""


    def displayName(self):
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


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        assignment = edit.richText(2,"Assignment",html.escape(self._p_assignment))
        return Package.displayView(self) + assignment


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = Package.editDefinitions(self)
        ret.append(edit.lineEdit("Assignment:","_p_assignment"))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return qtg.QIcon(":/python/object.svg")


    def inClass(self):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this object is part of a class or false otherwise.
        """
        return self.parent()._TYPE_ == "Access"


    def isArgument(self):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this object is an argument of a function or false otherwise.
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
        Package.setDefaultProperties(self)
        self._p_name = "object"
        self._p_assignment = ""
