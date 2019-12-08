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


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/cpp/template.svg")


    def display_name(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return self._p_name


    def display_view(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        type_ = ut.rich_text(2,"Type",html.escape(self._p_type))
        assignment = ut.rich_text(2,"Assignment",html.escape(self._p_assignment))
        return namespace.Base.display_view(self) + type_ + assignment


    def argument_view(self):
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
        return True


    def edit_definitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = namespace.Base.edit_definitions(self)
        ret.append(ut.lineEdit("Type:","_p_type"))
        ret.append(ut.lineEdit("Assignment:","_p_assignment"))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.set_default_properties(self)
        self._p_name = "Template"
        self._p_type = "class @"
        self._p_assignment = ""


    def clear_properties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.clear_properties(self)
        self._p_name = ""
        self._p_type = ""
        self._p_assignment = ""


    def build_comment(self, begin):
        """
        Detailed description.

        begin : Detailed description.
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


    def build_argument(self):
        """
        Detailed description.
        """
        ret = self._p_type.replace("@",self._p_name)
        if self._p_assignment:
            ret += "=" + self._p_assignment
        return ret
