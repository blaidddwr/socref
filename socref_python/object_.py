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


    def in_class(self):
        """
        Getter method.

        return : True if this object is part of a class or false otherwise.
        """
        return self.parent()._TYPE_ == "Access"


    def is_argument(self):
        """
        Getter method.

        return : True if this object is an argument of a function or false otherwise.
        """
        return self.parent()._TYPE_ == "Function"


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/python/object.svg")


    def display_name(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = self._p_name
        if self.is_argument() and self._p_assignment:
            ret += " ="
        return ret


    def display_view(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        assignment = ut.richText(2,"Assignment",html.escape(self._p_assignment))
        return package.Package.display_view(self) + assignment


    def argument_view(self):
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


    def build_list(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ()


    def is_volatile_above(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return self.is_argument()


    def edit_definitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = package.Package.edit_definitions(self)
        ret.append(ut.lineEdit("Assignment:","_p_assignment"))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        package.Package.set_default_properties(self)
        self._p_name = "object"
        self._p_assignment = ""


    def clear_properties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        package.Package.clear_properties(self)
        self._p_assignment = ""


    def argument(self):
        """
        Getter method.

        return : A string that is the source code for this object as a function argument.
        """
        ret = self._p_name
        if self._p_assignment:
            ret += "=" + self._p_assignment
        return ret


    def comment(self, begin):
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


    def space(self, previous):
        """
        Implements the .package.Package interface.

        previous : See interface docs.

        return : See interface docs.
        """
        ret = ""
        if self.in_class():
            if previous is None:
                ret = "\n"*settings.H3LINES
            elif previous._TYPE_ == "Function":
                ret = "\n"*settings.H2LINES
            else:
                ret = ""
        else:
            if previous is None:
                ret = "\n"*settings.H3LINES
            elif previous._TYPE_ == "Function":
                ret = "\n"*settings.H2LINES
            elif previous._TYPE_ == "Class":
                ret = "\n"*settings.H1LINES
            else:
                ret = ""
        return ret


    def build(self, definition, begin=""):
        """
        Implements the .package.Package interface.

        definition : See interface docs.

        begin : See interface docs.

        return : See interface docs.
        """
        ret = begin + "#\n"
        ret += ut.wrapBlocks(self._p_description,begin=begin + "# ",columns=settings.COLUMNS)
        ret += begin + "#\n"
        ret += begin + self._p_name
        if self._p_assignment:
            ret += " = " + self._p_assignment
        ret += "\n"
        return ret
