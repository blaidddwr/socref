"""
Contains the function block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import namespace








@register("Function")
class Block(namespace.Block):
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
        namespace.Block.__init__(self)
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


    def has_templates(self):
        """
        Getter method.

        return : True if this function has any templates or false otherwise.
        """
        for child in self:
            if child._TYPE_ == "Template":
                return True
        return False


    ##########################
    # PUBLIC - Basic Methods #
    ##########################


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
        ret = "<> " if self.has_templates() else ""
        if not self.is_constructor and not self.is_destructor and self._p_return_type != "void":
            ret += "... "
        ret += "%s(%i)%s" % (self._p_name,len(self),self.__flags_())
        return ret


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        self.__check_flags_()
        arguments = ""
        return_ = ""
        templates = ""
        flags = ""
        return namespace.Block.display_view(self) + arguments + return_ + templates + flags


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


    #############################
    # PUBLIC - Property Methods #
    #############################


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = namespace.Block.edit_definitions(self)
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
        namespace.Block.set_default_properties(self)
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
        namespace.Block.clear_properties(self)
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

        return : A bracket enclosed list of flags this block has enabled. If this block has no flags
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
        if ret:
            ret = " [%s]" % ret
        return ret
