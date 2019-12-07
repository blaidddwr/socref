"""
Contains the function block definition and generic templatee block.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import namespace








class Templatee(namespace.Base):
    """
    This is the templatee class. It provides methods for handling template block children. This is
    meant to act as a base class for any block that can have templates.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new templatee block.
        """
        namespace.Base.__init__(self)


    ####################
    # PUBLIC - Methods #
    ####################


    def has_templates(self):
        """
        Getter method.

        return : True if this block has any templates or false otherwise.
        """
        for child in self:
            if child._TYPE_ == "Template":
                return True
        return False


    #######################
    # PROTECTED - Methods #
    #######################


    def _template_name_(self):
        """
        Getter method.

        return : A string that is a decoration for a block's display name providing information
                 about its templates. If this block has no templates then an empty string is
                 returned.
        """
        return "<> " if self.has_templates() else ""


    def _templates_view_(self):
        """
        Getter method.

        return : A rich text string providing detailed information about this block's templates. If
                 this block has no templates then an empty string is returned.
        """
        return ut.rich_text(
            2
            ,"Templates"
            ,"".join((child.argument_view() for child in self if child._TYPE_ == "Template"))
        )








@register("Function")
class Block(Templatee):
    """
    This is the function block class. It implements the Socrates' Reference abstract block class. It
    represents a C++ function.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new function block.
        """
        Templatee.__init__(self)
        self._p_return_type = ""
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_default = "0"
        self._p_deleted = "0"
        self._p_explicit = "0"
        self._p_virtual = "0"
        self._p_const = "0"
        self._p_static = "0"
        self._p_noexcept = "0"
        self._p_override = "0"
        self._p_final = "0"
        self._p_abstract = "0"


    ####################
    # PUBLIC - Methods #
    ####################


    def is_constructor(self):
        """
        Getter method.

        return : True if this function is a constructor or false otherwise.
        """
        return self._p_name == "^"


    def is_destructor(self):
        """
        Getter method.

        return : True if this function is a destructor or false otherwise.
        """
        return self._p_name == "~^"


    def is_operator(self):
        """
        Getter method.

        return : True if this function is an operator or false otherwise.
        """
        return self._p_name.startswith("operator")


    def is_default(self):
        """
        Getter method.

        return : True if this function is the default or false otherwise.
        """
        return bool(int(self._p_default))


    def is_deleted(self):
        """
        Getter method.

        return : True if this function is deleted or false otherwise.
        """
        return bool(int(self._p_deleted))


    def is_explicit(self):
        """
        Getter method.

        return : True if this function is an explicit constructor or false otherwise.
        """
        return bool(int(self._p_explicit))


    def is_const(self):
        """
        Getter method.

        return : True if this function is constant or false otherwise.
        """
        return bool(int(self._p_const))


    def is_static(self):
        """
        Getter method.

        return : True if this function is static or false otherwise.
        """
        return bool(int(self._p_static))


    def is_noexcept(self):
        """
        Getter method.

        return : True if this function does not throw exceptions or false otherwise.
        """
        return bool(int(self._p_noexcept))


    def is_virtual(self):
        """
        Getter method.

        return : True if this function is virtual or false otherwise.
        """
        return bool(int(self._p_virtual))


    def is_override(self):
        """
        Getter method.

        return : True if this function is overriding a virtual interface or false otherwise.
        """
        return bool(int(self._p_override))


    def is_final(self):
        """
        Getter method.

        return : True if this function is the final overriding virtual interface or false otherwise.
        """
        return bool(int(self._p_final))


    def is_abstract(self):
        """
        Getter method.

        return : True if this function is abstract or false otherwise.
        """
        return bool(int(self._p_abstract))


    def is_method(self):
        """
        Getter method.

        return : True if this function is a method or false otherwise.
        """
        return self.parent()._TYPE_ == "Access"


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        if self.is_constructor():
            return qtg.QIcon(":/cpp/constructor.svg")
        elif self.is_destructor():
            if self.is_abstract():
                return qtg.QIcon(":/cpp/abstract_destructor.svg")
            elif self.is_virtual():
                return qtg.QIcon(":/cpp/virtual_destructor.svg")
            else:
                return qtg.QIcon(":/cpp/destructor.svg")
        elif self.is_operator():
            if self.is_abstract():
                return qtg.QIcon(":/cpp/abstract_operator.svg")
            elif self.is_virtual():
                return qtg.QIcon(":/cpp/virtual_operator.svg")
            else:
                return qtg.QIcon(":/cpp/operator.svg")
        else:
            if self.is_abstract():
                return qtg.QIcon(":/cpp/abstract_function.svg")
            elif self.is_virtual():
                return qtg.QIcon(":/cpp/virtual_function.svg")
            elif self.is_static():
                return qtg.QIcon(":/cpp/static_function.svg")
            else:
                return qtg.QIcon(":/cpp/function.svg")


    def display_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = self._template_name_()
        if not self.is_constructor() and not self.is_destructor() and self._p_return_type != "void":
            ret += "... "
        flags = self.__flags_()
        if flags:
            flags = " [%s]" % flags
        ret += "%s(%i)%s" % (self._p_name,len(self),flags)
        return ret


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        self.__check_flags_()
        return_ = ""
        if self._p_return_type != "void":
            return_ = "<p><b>%s</b> : %s</p>" % (self._p_return_type,self._p_return_description)
        return_ = ut.rich_text(2,"Return",return_)
        flags = ut.rich_text_list(2,"Flags",self.__flags_list_())
        return (
            namespace.Base.display_view(self)
            + self._templates_view_()
            + self.__arguments_view_()
            + return_
            + flags
        )


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ("Template","Variable")


    def is_volatile_above(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return self.is_method()


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = namespace.Base.edit_definitions(self)
        ret.append(ut.line_edit("Return Type:","_p_return_type"))
        ret.append(ut.text_edit("Return Description:","_p_return_description",speller=True))
        ret.append(ut.checkbox_edit("No Exceptions","_p_noexcept"))
        if self.is_method():
            ret.append(ut.checkbox_edit("Default","_p_default"))
            ret.append(ut.checkbox_edit("Deleted","_p_deleted"))
            ret.append(ut.checkbox_edit("Explicit","_p_explicit"))
            ret.append(ut.checkbox_edit("Constant","_p_const"))
            ret.append(ut.checkbox_edit("Static","_p_static"))
            ret.append(ut.checkbox_edit("Virtual","_p_virtual"))
            ret.append(ut.checkbox_edit("Override","_p_override"))
            ret.append(ut.checkbox_edit("Final","_p_final"))
            ret.append(ut.checkbox_edit("Abstract","_p_abstract"))
        else:
            ret.append(ut.hidden_edit("_p_default","0"))
            ret.append(ut.hidden_edit("_p_deleted","0"))
            ret.append(ut.hidden_edit("_p_explicit","0"))
            ret.append(ut.hidden_edit("_p_const","0"))
            ret.append(ut.hidden_edit("_p_static","0"))
            ret.append(ut.hidden_edit("_p_virtual","0"))
            ret.append(ut.hidden_edit("_p_override","0"))
            ret.append(ut.hidden_edit("_p_final","0"))
            ret.append(ut.hidden_edit("_p_abstract","0"))
        ret.append(ut.text_edit("Inline Comments:","_p_inlines"))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        namespace.Base.set_default_properties(self)
        self._p_name = "function"
        self._p_return_type = "void"
        self._p_return_description = "Detailed Description."
        self._p_inlines = ""
        self._p_default = "0"
        self._p_deleted = "0"
        self._p_explicit = "0"
        self._p_virtual = "0"
        self._p_const = "0"
        self._p_static = "0"
        self._p_noexcept = "0"
        self._p_override = "0"
        self._p_final = "0"
        self._p_abstract = "0"


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        namespace.Base.clear_properties(self)
        self._p_return_type = ""
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_default = "0"
        self._p_deleted = "0"
        self._p_explicit = "0"
        self._p_virtual = "0"
        self._p_const = "0"
        self._p_static = "0"
        self._p_noexcept = "0"
        self._p_override = "0"
        self._p_final = "0"
        self._p_abstract = "0"


    #####################
    # PRIVATE - Methods #
    #####################


    def __check_flags_(self):
        """
        Sets this function's flags to legal values if it is not a method.
        """
        if not self.is_method():
            self._p_default = "0"
            self._p_deleted = "0"
            self._p_explicit = "0"
            self._p_virtual = "0"
            self._p_const = "0"
            self._p_static = "0"
            self._p_override = "0"
            self._p_final = "0"
            self._p_abstract = "0"


    def __flags_(self):
        """
        Getter method.

        return : A string of character flags this block has enabled. If this block has no flags
                 enabled then an empty string is returned.
        """
        ret = ""
        if self.is_default():
            ret += "D"
        if self.is_deleted():
            ret += "R"
        if self.is_explicit():
            ret += "E"
        if self.is_const():
            ret += "C"
        if self.is_static():
            ret += "S"
        if self.is_noexcept():
            ret += "N"
        if self.is_virtual():
            ret += "V"
        if self.is_override():
            ret += "O"
        if self.is_final():
            ret += "F"
        if self.is_abstract():
            ret += "A"
        return ret


    def __arguments_view_(self):
        """
        Getter method.

        return : Rich text detailed view of all this function's arguments. If this function has no
                 arguments then this returns an empty string.
        """
        return ut.rich_text(
            2
            ,"Arguments"
            ,"".join((child.argument_view() for child in self if child._TYPE_ == "Variable"))
        )


    def __flags_list_(self):
        """
        Getter method.

        return : A string list of flags this block has enabled. If this block has no flags enabled
                 then an empty string is returned.
        """
        flags = []
        if self.is_default():
            flags.append("Default")
        if self.is_deleted():
            flags.append("Deleted")
        if self.is_explicit():
            flags.append("Explicit")
        if self.is_const():
            flags.append("Constant")
        if self.is_static():
            flags.append("Static")
        if self.is_noexcept():
            flags.append("No Exceptions")
        if self.is_virtual():
            flags.append("Virtual")
        if self.is_override():
            flags.append("Override")
        if self.is_final():
            flags.append("Final")
        if self.is_abstract():
            flags.append("Abstract")
        return flags
