"""
Contains the variable block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import settings
from . import template








@register("Variable")
class Variable(template.Template):
    """
    This is the variable block class. It implements the Socrates' Reference abstract block class. It
    represents a C++ variable.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new variable block.
        """
        template.Template.__init__(self)
        self._p_constexpr = "0"
        self._p_static = "0"
        self._p_mutable = "0"
        self._p_thread_local = "0"


    ####################
    # PUBLIC - Methods #
    ####################


    def is_constexpr(self):
        """
        Getter method.

        return : True if this variable is a constant expression or false otherwise.
        """
        return bool(int(self._p_constexpr))


    def is_static(self):
        """
        Getter method.

        return : True if this variable is static or false otherwise.
        """
        return bool(int(self._p_static))


    def is_mutable(self):
        """
        Getter method.

        return : True if this variable is mutable or false otherwise.
        """
        return bool(int(self._p_mutable))


    def is_thread_local(self):
        """
        Getter method.

        return : True if this variable is thread local or false otherwise.
        """
        return bool(int(self._p_thread_local))


    def is_argument(self):
        """
        Getter method.

        return : True if this variable is an argument of a function or false otherwise.
        """
        return self.parent()._TYPE_ == "Function"


    def in_class(self):
        """
        Getter method.

        return : True if this variable is part of a class or false otherwise.
        """
        return self.parent()._TYPE_ == "Access"


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        if self.is_static():
            return qtg.QIcon(":/cpp/static_variable.svg")
        else:
            return qtg.QIcon(":/cpp/variable.svg")


    def display_name(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        base = self._p_name
        if self._p_assignment:
            if self.is_argument():
                base += " ="
            else:
                base += " {}"
        flags = self.__flags_()
        if flags:
            flags = " [%s]" % flags
        return base + flags


    def display_view(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        self.__check_flags_()
        return template.Block.display_view(self) + self.__flags_view_()


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
        ret = template.Block.edit_definitions(self)
        if not self.is_argument():
            ret.append(ut.checkboxEdit("Constant Expression","_p_constexpr"))
            ret.append(ut.checkboxEdit("Thread Local","_p_thread_local"))
        else:
            ret.append(ut.hidden_edit("_p_constexpr","0"))
            ret.append(ut.hidden_edit("_p_thread_local","0"))
        if self.in_class():
            ret.append(ut.checkboxEdit("Static","_p_static"))
            ret.append(ut.checkboxEdit("Mutable","_p_mutable"))
        else:
            ret.append(ut.hidden_edit("_p_static","0"))
            ret.append(ut.hidden_edit("_p_mutable","0"))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        template.Template.set_default_properties(self)
        self._p_name = "variable"
        self._p_type = "int @"
        self._p_constexpr = "0"
        self._p_static = "0"
        self._p_mutable = "0"
        self._p_thread_local = "0"


    def clear_properties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        template.Template.clear_properties(self)
        self._p_constexpr = "0"
        self._p_static = "0"
        self._p_mutable = "0"
        self._p_thread_local = "0"


    def build_declaration(self, begin, template):
        """
        Detailed description.

        begin : Detailed description.

        template : Detailed description.
        """
        ret = "\n"*settings.H2LINES
        ret += (
            begin
            + "/*!\n"
            + ut.wrapBlocks(self._p_description,begin+" * ",begin+" *",settings.COLUMNS)
            + begin
            + " */\n"
        )
        ret += begin
        if self.is_static():
            ret += "static "
        if self.is_constexpr():
            ret += "constexpr "
        if self.is_mutable():
            ret += "mutable "
        if self.is_thread_local():
            ret += "thread_local "
        ret += self._p_type.replace("@",self._p_name)
        if self._p_assignment and (self.is_constexpr() or not self.is_static()):
            ret += " {%s}" % self._p_assignment
        ret += ";\n"
        return ret


    def build_definition(self, scope):
        """
        Detailed description.

        scope : Detailed description.
        """
        if self._p_assignment and self.is_static() and not self.is_constexpr():
            if scope:
                scope += "::"
            return self._p_type.replace("@",scope+self._p_name) + " {%s};\n"%self._p_assignment
        else:
            return ""


    #####################
    # PRIVATE - Methods #
    #####################


    def __check_flags_(self):
        """
        Sets this variable's flags to legal values if it is an argument or not part of a class.
        """
        if self.is_argument():
            self._p_constexpr = "0"
            self._p_thread_local = "0"
        if not self.in_class():
            self._p_static = "0"
            self._p_mutable = "0"


    def __flags_(self):
        """
        Getter method.

        return : A string of character flags this block has enabled. If this block has no flags
                 enabled then an empty string is returned.
        """
        ret = ""
        if self.is_constexpr():
            ret += "X"
        if self.is_static():
            ret += "S"
        if self.is_mutable():
            ret += "M"
        if self.is_thread_local():
            ret += "L"
        return ret


    def __flags_view_(self):
        """
        Getter method.

        return : Rich text list of flags this block has enabled. If this block has no flags enabled
                 then an empty string is returned.
        """
        flags = []
        if self.is_constexpr():
            flags.append("Constant Expression")
        if self.is_static():
            flags.append("Static")
        if self.is_mutable():
            flags.append("Mutable")
        if self.is_thread_local():
            flags.append("Thread Local")
        return ut.rich_text_list(2,"Flags",flags)
