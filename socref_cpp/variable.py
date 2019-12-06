"""
Contains the variable block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import template








@register("Variable")
class Block(template.Block):
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
        template.Block.__init__(self)
        self._p_constexpr = "0"
        self._p_static = "0"
        self._p_mutable = "0"
        self._p_threadlocal = "0"


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
        return bool(int(self._p_threadlocal))


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


    ##########################
    # PUBLIC - Basic Methods #
    ##########################


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        if self.is_static():
            return qtg.QIcon(":/cpp/static_variable.svg")
        else:
            return qtg.QIcon(":/cpp/variable.svg")


    def display_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        base = self._p_name
        if self._p_assignment:
            base += " ="
        return base + self.__flags_()


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        self.__check_flags_()
        return template.Block.display_view(self) + self.__flags_view_()


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ()


    def is_volatile_above(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return self.is_argument()


    #############################
    # PUBLIC - Property Methods #
    #############################


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = template.Block.edit_definitions(self)
        ret.append(ut.checkbox_edit("Constant Expression","_p_constexpr"))
        ret.append(ut.checkbox_edit("Thread Local","_p_threadlocal"))
        if self.in_class():
            ret.append(ut.checkbox_edit("Static","_p_static"))
            ret.append(ut.checkbox_edit("Mutable","_p_mutable"))
        else:
            ret.append(ut.hidden_edit("_p_static","0"))
            ret.append(ut.hidden_edit("_p_mutable","0"))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        template.Block.set_default_properties(self)
        self._p_name = "variable"
        self._p_type = "void @"
        self._p_constexpr = "0"
        self._p_static = "0"
        self._p_mutable = "0"
        self._p_threadlocal = "0"


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        template.Block.clear_properties(self)
        self._p_constexpr = "0"
        self._p_static = "0"
        self._p_mutable = "0"
        self._p_threadlocal = "0"


    #####################
    # PRIVATE - Methods #
    #####################


    def __check_flags_(self):
        """
        Sets this variable's flags to legal values if it is not part of a class.
        """
        if not self.in_class():
            self._p_static = "0"
            self._p_mutable = "0"


    def __flags_(self):
        """
        Getter method.

        return : A bracket enclosed list of flags this block has enabled. If this block has no flags
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
        if ret:
            ret = " [%s]" % ret
        return ret


    def __flags_view_(self):
        """
        Getter method.

        return : Rich text list of flags this block has enabled. If this block has no flags enabled
                 then an empty string is returned.
        """
        flags = ""
        if self.is_constexpr():
            flags += "<li>Constant Expression</li>"
        if self.is_static():
            flags += "<li>Static</li>"
        if self.is_mutable():
            flags += "<li>Mutable</li>"
        if self.is_thread_local():
            flags += "<li>Thread Local</li>"
        return ut.rich_text(2,"Flags",flags)
