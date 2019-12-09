"""
Contains the object block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from socref import abstract
from . import settings
from . import package








@register("Object")
class Object(package.Package):
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
        package.Package.__init__(self)
        self._p_assignment = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def argumentView(self):
        """
        Getter method.

        return : Rich text paragraph that describes this object as an argument of a function.
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
        Implements the .package.Package interface.

        definition : See interface docs.

        begin : See interface docs.

        return : See interface docs.
        """
        ret = [""]*settings.H3LINES
        blankCommentLine = begin + "#"
        ret.append(blankCommentLine)
        ret += ut.wrapBlocks(self._p_description,begin=begin + "# ",columns=settings.COLUMNS)
        ret.append(blankCommentLine)
        line = begin + self._p_name
        if self._p_assignment:
            line += " = " + self._p_assignment
        ret.append(line)
        return ret


    def buildArgument(self):
        """
        Getter method.

        return : A string that is the source code for this object as a function argument.
        """
        ret = self._p_name
        if self._p_assignment:
            ret += "=" + self._p_assignment
        return ret


    def buildComment(self, begin):
        """
        Detailed description.

        begin : The indent or begin string that is appended to every line of returned source code.

        return : A string that is the source code doc string fragment for this object as a function
                 argument.
        """
        initial = self._p_name + " : "
        return ut.wrapText(
            initial + self._p_description
            ,begin=begin
            ,after=" "*len(initial)
            ,columns=settings.COLUMNS
        )


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ()


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        package.Package.clearProperties(self)
        self._p_assignment = ""


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = self._p_name
        if self.isArgument() and self._p_assignment:
            ret += " ="
        return ret


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        assignment = ut.richText(2,"Assignment",html.escape(self._p_assignment))
        return package.Package.displayView(self) + assignment


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = package.Package.editDefinitions(self)
        ret.append(ut.lineEdit("Assignment:","_p_assignment"))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/python/object.svg")


    def inClass(self):
        """
        Getter method.

        return : True if this object is part of a class or false otherwise.
        """
        return self.parent()._TYPE_ == "Access"


    def isArgument(self):
        """
        Getter method.

        return : True if this object is an argument of a function or false otherwise.
        """
        return self.parent()._TYPE_ == "Function"


    def isVolatileAbove(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return self.isArgument()


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        package.Package.setDefaultProperties(self)
        self._p_name = "object"
        self._p_assignment = ""
