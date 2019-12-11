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


    def buildDeclaration(self, begin, blanks=True):
        """
        Implements the .namespace.Base interface with one additional optional argument.

        begin : See interface docs.

        blanks : True to include initial blank lines in the returned code separating this variable
                 declaration from others or false for no blank lines.

        return : See interface docs.
        """
        ret = []
        if blanks:
            ret += [""]*settings.H2LINES
        if self.inUnion():
            ret += ut.wrapText(self._p_description,begin+"/// ",columns=settings.COLUMNS)
        else:
            ret.append(begin + "/*!")
            ret += ut.wrapText(self._p_description,begin+" * ",columns=settings.COLUMNS)
            ret.append(begin + " */")
        line = begin+self.__buildFlags_()+self._p_type.replace("@",self._p_name)
        if (
            self._p_assignment
            and not self.inUnion()
            and (self.isConstExpr() or not self.isStatic())
        ):
            line += " {%s}"%self._p_assignment
        ret.append("".join(line) + ";")
        return ret


    def buildDefinition(self, definitions, scope, template, header):
        """
        Implements the .namespace.Base interface.

        definitions : See interface docs.

        scope : See interface docs.

        template : See interface docs.

        header : See interface docs.

        return : See interface docs.
        """
        if (
            ((not template and not header) or (template and header))
            and self._p_assignment
            and self.isStatic()
            and not self.isConstExpr()
        ):
            if scope:
                scope += "::"
            return ([self._p_type.replace("@",scope+self._p_name)+" {%s};"%self._p_assignment],[])
        else:
            return ([],[])


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
        template.Template.clearProperties(self)
        self._p_constexpr = "0"
        self._p_static = "0"
        self._p_mutable = "0"
        self._p_thread_local = "0"


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        base = self._p_name
        if self._p_assignment:
            if self.isArgument():
                base += " ="
            else:
                base += " {}"
        flags = self.__flags_()
        if flags:
            flags = " [%s]" % flags
        return base + flags


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        self.__checkFlags_()
        return template.Template.displayView(self) + self.__flagsView_()


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = template.Template.editDefinitions(self)
        if not self.isArgument():
            ret.append(ut.checkboxEdit("Constant Expression","_p_constexpr"))
            ret.append(ut.checkboxEdit("Thread Local","_p_thread_local"))
        else:
            ret.append(ut.hiddenEdit("_p_constexpr","0"))
            ret.append(ut.hiddenEdit("_p_thread_local","0"))
        if self.inClass():
            ret.append(ut.checkboxEdit("Static","_p_static"))
            ret.append(ut.checkboxEdit("Mutable","_p_mutable"))
        else:
            ret.append(ut.hiddenEdit("_p_static","0"))
            ret.append(ut.hiddenEdit("_p_mutable","0"))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        if self.isStatic():
            return qtg.QIcon(":/cpp/static_variable.svg")
        else:
            return qtg.QIcon(":/cpp/variable.svg")


    def inClass(self):
        """
        Getter method.

        return : True if this variable is part of a class or false otherwise.
        """
        return self.parent()._TYPE_ == "Access"


    def inUnion(self):
        """
        Detailed description.
        """
        return self.parent()._TYPE_ == "Union"


    def isArgument(self):
        """
        Getter method.

        return : True if this variable is an argument of a function or false otherwise.
        """
        return self.parent()._TYPE_ == "Function"


    def isConstExpr(self):
        """
        Getter method.

        return : True if this variable is a constant expression or false otherwise.
        """
        return bool(int(self._p_constexpr))


    def isMutable(self):
        """
        Getter method.

        return : True if this variable is mutable or false otherwise.
        """
        return bool(int(self._p_mutable))


    def isStatic(self):
        """
        Getter method.

        return : True if this variable is static or false otherwise.
        """
        return bool(int(self._p_static))


    def isThreadLocal(self):
        """
        Getter method.

        return : True if this variable is thread local or false otherwise.
        """
        return bool(int(self._p_thread_local))


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
        template.Template.setDefaultProperties(self)
        self._p_name = "variable"
        self._p_type = "int @"
        self._p_constexpr = "0"
        self._p_static = "0"
        self._p_mutable = "0"
        self._p_thread_local = "0"


    #####################
    # PRIVATE - Methods #
    #####################


    def __buildFlags_(self):
        """
        Getter method.

        return : A string that is the code declarations for any flags this variable has enabled.
        """
        ret = ""
        if self.isStatic():
            ret += "static "
        if self.isConstExpr():
            ret += "constexpr "
        if self.isMutable():
            ret += "mutable "
        if self.isThreadLocal():
            ret += "thread_local "
        return ret


    def __checkFlags_(self):
        """
        Sets this variable's flags to legal values if it is an argument or not part of a class.
        """
        if self.isArgument():
            self._p_constexpr = "0"
            self._p_thread_local = "0"
        if not self.inClass():
            self._p_static = "0"
            self._p_mutable = "0"


    def __flags_(self):
        """
        Getter method.

        return : A string of character flags this block has enabled. If this block has no flags
                 enabled then an empty string is returned.
        """
        ret = ""
        if self.isConstExpr():
            ret += "X"
        if self.isStatic():
            ret += "S"
        if self.isMutable():
            ret += "M"
        if self.isThreadLocal():
            ret += "L"
        return ret


    def __flagsView_(self):
        """
        Getter method.

        return : Rich text list of flags this block has enabled. If this block has no flags enabled
                 then an empty string is returned.
        """
        flags = []
        if self.isConstExpr():
            flags.append("Constant Expression")
        if self.isStatic():
            flags.append("Static")
        if self.isMutable():
            flags.append("Mutable")
        if self.isThreadLocal():
            flags.append("Thread Local")
        return ut.richTextList(2,"Flags",flags)
