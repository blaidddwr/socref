"""
Contains the template block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import settings
from . import namespace








@register("Template")
class Template(namespace.Base):
    """
    This is the template block class. It implements the Socrates' Reference abstract block class. It
    represents a C++ template.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new namespace block.
        """
        namespace.Base.__init__(self)
        self._p_type = ""
        self._p_assignment = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def argumentView(self):
        """
        Getter method.

        return : Rich text paragraph that describes this template as an argument.
        """
        ret = "<p><b>%s " % html.escape(self._p_type.replace("@",self._p_name))
        if self._p_assignment:
            ret += " =</b> " + html.escape(self._p_assignment) + " : "
        else:
            ret += "</b> : "
        ret += html.escape(self._p_description) + "</p>"
        return ret


    def buildArgument(self):
        """
        Getter method.

        return : A source code fragment string that is the argument of this template.
        """
        ret = self._p_type.replace("@",self._p_name)
        if self._p_assignment:
            ret += "=" + self._p_assignment
        return ret


    def buildComment(self, begin):
        """
        Getter method.

        begin : A string that is added to the beginning of returned lines of comment code.

        return : A list of source code lines that is a comment fragment for this template as an
                 argument. This returns the correct doxygen syntax based off this block being an
                 actual template or variable argument.
        """
        header = ""
        if self._TYPE_ == "Template":
            header = "@tparam %s : " % self._p_name
        else:
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

        return : See interface docs.
        """
        return ()


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.clearProperties(self)
        self._p_name = ""
        self._p_type = ""
        self._p_assignment = ""


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return self._p_name


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        type_ = ut.richText(2,"Type",html.escape(self._p_type))
        assignment = ut.richText(2,"Assignment",html.escape(self._p_assignment))
        return namespace.Base.displayView(self) + type_ + assignment


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = namespace.Base.editDefinitions(self)
        ret.append(ut.lineEdit("Type:","_p_type"))
        ret.append(ut.lineEdit("Assignment:","_p_assignment"))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/cpp/template.svg")


    def isVolatileAbove(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return True


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.setDefaultProperties(self)
        self._p_name = "Template"
        self._p_type = "class @"
        self._p_assignment = ""
