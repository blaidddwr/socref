"""
Contains the function block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from socref import abstract
from . import settings
from . import package
from . import access








class Descriptor(package.Block):
    """
    This is the python descriptor class. It partially implements the Socrates' Reference abstract
    block class. This provides a descriptors property along with utility methods for it. This is
    meant to act as a base class for any block that has descriptors.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new descriptor block.
        """
        package.Block.__init__(self)
        self._p_descriptors = ""


    #############################
    # PUBLIC - Property Methods #
    #############################


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        package.Block.set_default_properties(self)
        self._p_descriptors = ""


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        package.Block.clear_properties(self)
        self._p_descriptors = ""


    #######################
    # PROTECTED - Methods #
    #######################


    def _descriptors_name_(self):
        """
        Getter method.

        return : A string that is a decoration for a block's display name providing information
                 about its descriptors. If this block has no descriptors then an empty string is
                 returned.
        """
        if self._p_descriptors:
            return " " + "@" * len(['' for line in self._p_descriptors.split("\n") if line])
        else:
            return ""


    def _descriptors_view_(self):
        """
        Getter method.

        return : A rich text string providing detailed information about this block's descriptors.
                 If this block has no descriptors then an empty string is returned.
        """
        ret = "<br/>".join(
            ("@" + html.escape(line) for line in self._p_descriptors.split("\n") if line)
        )
        if ret:
            ret = "<h2>Descriptors</h2><p>%s</p>" % ret
        return ret


    def _descriptors_edit_definition_(self):
        """
        Getter Method.

        return : The edit definition for this block's descriptors property.
        """
        return ut.text_edit("Descriptors:","_p_descriptors")


    def _build_descriptors_(self, begin):
        """
        Getter method.

        begin : The indent or begin string that is appended to every line of returned code.

        return : A string that is the source code for this block's descriptors. If this block has no
                 descriptors an empty string is returned.
        """
        ret = "\n".join((begin + "@" + line for line in self._p_descriptors.split("\n") if line))
        if ret:
            ret += "\n"
        return ret








@register("Function")
class Block(Descriptor):
    """
    This is the function block class. It implements the Socrates' Reference abstract block class. It
    represents a python function.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new function block.
        """
        Descriptor.__init__(self)
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_static = "0"
        self._p_abstract = "0"


    ####################
    # PUBLIC - Methods #
    ####################


    def is_static(self):
        """
        Getter method.

        return : True if this function is static or false otherwise.
        """
        return bool(int(self._p_static))


    def is_abstract(self):
        """
        Getter method.

        return : True if this function is abstract or false otherwise.
        """
        return bool(int(self._p_abstract))


    def is_method(self):
        """
        Getter method.

        return : True if this function is a method of a class or false otherwise.
        """
        return isinstance(self.parent(),access.Block)


    ##########################
    # PUBLIC - Basic Methods #
    ##########################


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        if self._p_name.startswith("__") and self._p_name.endswith("__"):
            return qtg.QIcon(":/python/operator.svg")
        elif self.is_static():
            return qtg.QIcon(":/python/static_function.svg")
        elif self.is_abstract():
            return qtg.QIcon(":/python/abstract_function.svg")
        else:
            return qtg.QIcon(":/python/function.svg")


    def display_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = ""
        if self._p_return_description:
            ret += "... "
        ret += "%s(%i)%s" % (self._p_name,len(self),self._descriptors_name_())
        return ret


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        self.__check_flags_()
        return_ = ut.rich_text(2,"Return",html.escape(self._p_return_description))
        return (
            package.Block.display_view(self)
            + self.__arguments_view_()
            + return_
            + self.__flags_view_()
            + self._descriptors_view_()
        )


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ("Object",)


    #############################
    # PUBLIC - Property Methods #
    #############################


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = package.Block.edit_definitions(self)
        ret.append(ut.text_edit("Return:","_p_return_description",speller=True))
        if self.is_method():
            ret.append(ut.checkbox_edit("Static","_p_static"))
            ret.append(ut.checkbox_edit("Abstract","_p_abstract"))
        else:
            ret.append(ut.hidden_edit("_p_static","0"))
            ret.append(ut.hidden_edit("_p_abstract","0"))
        ret.append(ut.text_edit("Inline Comments:","_p_inlines",speller=True))
        ret.append(self._descriptors_edit_definition_())
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        Descriptor.set_default_properties(self)
        self._p_name = "function"
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_static = "0"
        self._p_abstract = "0"


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        Descriptor.clear_properties(self)
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_static = "0"
        self._p_abstract = "0"


    ############################
    # PUBLIC - Parsing Methods #
    ############################


    def space(self, previous):
        """
        Implements the .package.Block interface.

        previous : See interface docs.

        return : See interface docs.
        """
        ret = ""
        if self.is_method():
            ret = "\n"*settings.H3LINES
        else:
            if previous is not None and previous._TYPE_ == "Class":
                ret = "\n"*settings.H1LINES
            else:
                ret = "\n"*settings.H2LINES
        return ret


    def build(self, definition, begin=""):
        """
        Implements the .package.Block interface.

        definition : See interface docs.

        begin : See interface docs.

        return : See interface docs.
        """
        ret = self.__build_header_(begin)
        ret += self.__build_doc_string_(begin)
        ret += self.__build_lines_(definition["functions"].pop(self._p_name,[]),begin)
        return ret


    #####################
    # PRIVATE - Methods #
    #####################


    def __check_flags_(self):
        """
        Sets this function's flags to legal values if it is not a method.
        """
        if not self.is_method():
            self._p_static = "0"
            self._p_abstract = "0"


    def __arguments_view_(self):
        """
        Getter method.

        return : Rich text detailed view of all this function's arguments. If this function has no
                 arguments then this returns an empty string.
        """
        ret = ""
        for arg in self:
            ret += arg.argument_view()
        if ret:
            ret = "<h2>Arguments</h2>" + ret
        return ret


    def __flags_view_(self):
        """
        Getter method.

        return : Rich text list of flags this block has enabled. If this block has no flags enabled
                 then an empty string is returned.
        """
        flags = ""
        if self.is_static():
            flags += "<li>Static</li>"
        if self.is_abstract():
            flags += "<li>Abstract</li>"
        return ut.rich_text(2,"Flags",flags)


    def __build_header_(self, begin):
        """
        Getter method.

        begin : The indent or begin string that is appended to every line of returned code.

        return : A string that is the source code of this function's header. The header includes any
                 descriptors and the define line.
        """
        ret = self._build_descriptors_(begin)
        if self.is_abstract():
            ret += begin + "@%s\n" % settings.ABSTRACT_DESCRIPTOR
        arguments = [arg.argument() for arg in self]
        if self.is_method() and not self.is_static():
            arguments.insert(0,"self")
        ret += "%sdef %s(%s):\n" % (begin,self._p_name,", ".join(arguments))
        return ret


    def __build_doc_string_(self, begin):
        """
        Getter method.

        begin : The indent or begin string that is appended to every line of returned code.

        return : A string that is the source code doc string of this function.
        """
        ret = begin + " "*settings.INDENT + '"""\n'
        ret += ut.wrap_blocks(
            self._p_description
            ,begin=begin + " "*settings.INDENT
            ,columns=settings.COLUMNS
        )
        for arg in self:
            ret += "\n" + arg.comment(begin + " "*settings.INDENT)
        if self._p_return_description:
            initial = "return : "
            ret += (
                "\n"
                + ut.wrap_text(
                    initial + self._p_return_description
                    ,begin=begin + " "*settings.INDENT
                    ,after=" "*len(initial)
                    ,columns=settings.COLUMNS
                )
            )
        ret += '%s%s"""\n' % (begin," "*settings.INDENT)
        return ret


    def __build_lines_(self, lines, begin):
        """
        Getter method.

        lines : A list of code lines used to generate the returned source code lines.

        begin : The indent or begin string that is appended to every line of returned code.

        return : A string that is the source code lines generated from the given list of code lines
                 and this function's inline comments.
        """
        if not lines:
            return begin + " "*settings.INDENT + "pass\n"
        ret = ""
        inlines = [line for line in self._p_inlines.split("\n\n") if line]
        for line in lines:
            if line.endswith("#"):
                if inlines:
                    ret += begin + " "*settings.INDENT + line + "\n"
                    ret += ut.wrap_text(
                        inlines.pop(0)
                        ,begin=begin + " "*settings.INDENT + line + " "
                        ,columns=settings.COLUMNS
                    )
                    ret += begin + " "*settings.INDENT + line + "\n"
                else:
                    ret += begin + " "*settings.INDENT + line + "\n"
            else:
                ret += begin + " "*settings.INDENT + line + "\n"
        return ret
