"""
Contains the function block definition and generic templatee block.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import settings
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


    def template_declaration(self):
        """
        Detailed description.
        """
        args = []
        for child in self:
            if child._TYPE_ == "Template":
                args.append(child.build_argument())
        if args:
            return "template<%s> " % ", ".join(args)
        else:
            return ""


    def template_scope(self):
        """
        Detailed description.
        """
        args = []
        for child in self:
            if child._TYPE_ == "Template":
                args.append(child._p_name)
        if args:
            return "<%s>" % ",".join(args)
        else:
            return ""


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
        return ut.richText(
            2
            ,"Templates"
            ,"".join((child.argumentView() for child in self if child._TYPE_ == "Template"))
        )








@register("Function")
class Function(Templatee):
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


    def isStatic(self):
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


    def isAbstract(self):
        """
        Getter method.

        return : True if this function is abstract or false otherwise.
        """
        return bool(int(self._p_abstract))


    def isMethod(self):
        """
        Getter method.

        return : True if this function is a method or false otherwise.
        """
        return self.parent()._TYPE_ == "Access"


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        if self.is_constructor():
            return qtg.QIcon(":/cpp/constructor.svg")
        elif self.is_destructor():
            if self.isAbstract():
                return qtg.QIcon(":/cpp/abstract_destructor.svg")
            elif self.is_virtual():
                return qtg.QIcon(":/cpp/virtual_destructor.svg")
            else:
                return qtg.QIcon(":/cpp/destructor.svg")
        elif self.is_operator():
            if self.isAbstract():
                return qtg.QIcon(":/cpp/abstract_operator.svg")
            elif self.is_virtual():
                return qtg.QIcon(":/cpp/virtual_operator.svg")
            else:
                return qtg.QIcon(":/cpp/operator.svg")
        else:
            if self.isAbstract():
                return qtg.QIcon(":/cpp/abstract_function.svg")
            elif self.is_virtual():
                return qtg.QIcon(":/cpp/virtual_function.svg")
            elif self.isStatic():
                return qtg.QIcon(":/cpp/static_function.svg")
            else:
                return qtg.QIcon(":/cpp/function.svg")


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = self._template_name_()
        if not self.is_constructor() and not self.is_destructor() and self._p_return_type != "void":
            ret += "... "
        flags = self.__flags_()
        if flags:
            flags = " [%s]" % flags
        ret += "%s(%i)%s" % (self.__name_(),len(self),flags)
        return ret


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        self.__checkFlags_()
        return_ = ""
        if self._p_return_type != "void":
            return_ = "<p><b>%s</b> : %s</p>" % (self._p_return_type,self._p_return_description)
        return_ = ut.richText(2,"Return",return_)
        flags = ut.richText_list(2,"Flags",self.__flags_list_())
        return (
            namespace.Base.displayView(self)
            + self._templates_view_()
            + self.__argumentsView_()
            + return_
            + flags
        )


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Template","Variable")


    def isVolatileAbove(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return self.isMethod()


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = namespace.Base.editDefinitions(self)
        ret.append(ut.lineEdit("Return Type:","_p_return_type"))
        ret.append(ut.textEdit("Return Description:","_p_return_description",speller=True))
        ret.append(ut.checkboxEdit("No Exceptions","_p_noexcept"))
        if self.isMethod():
            ret.append(ut.checkboxEdit("Default","_p_default"))
            ret.append(ut.checkboxEdit("Deleted","_p_deleted"))
            ret.append(ut.checkboxEdit("Explicit","_p_explicit"))
            ret.append(ut.checkboxEdit("Constant","_p_const"))
            ret.append(ut.checkboxEdit("Static","_p_static"))
            ret.append(ut.checkboxEdit("Virtual","_p_virtual"))
            ret.append(ut.checkboxEdit("Override","_p_override"))
            ret.append(ut.checkboxEdit("Final","_p_final"))
            ret.append(ut.checkboxEdit("Abstract","_p_abstract"))
        else:
            ret.append(ut.hiddenEdit("_p_default","0"))
            ret.append(ut.hiddenEdit("_p_deleted","0"))
            ret.append(ut.hiddenEdit("_p_explicit","0"))
            ret.append(ut.hiddenEdit("_p_const","0"))
            ret.append(ut.hiddenEdit("_p_static","0"))
            ret.append(ut.hiddenEdit("_p_virtual","0"))
            ret.append(ut.hiddenEdit("_p_override","0"))
            ret.append(ut.hiddenEdit("_p_final","0"))
            ret.append(ut.hiddenEdit("_p_abstract","0"))
        ret.append(ut.textEdit("Inline Comments:","_p_inlines"))
        return ret


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.setDefaultProperties(self)
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


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.clearProperties(self)
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


    def build_declaration(self, begin, template):
        """
        Detailed description.

        begin : Detailed description.

        template : Detailed description.
        """
        ret = "\n"*settings.H2LINES
        ret += begin + "/*!\n"
        ret += ut.wrapBlocks(self._p_description,begin+" * ",begin+" *",settings.COLUMNS)
        args = []
        for child in self:
            if child._TYPE_ == "Variable":
                args.append(child.build_argument())
                ret += begin + " *\n" + child.build_comment(begin+" * ")
        if self._p_return_type != "void":
            header = self._p_return_type + " : "
            ret += (
                begin
                + " *\n"
                + ut.wrapText(header + self._p_return_description,begin+" * "," "*len(header),100)
            )
        ret += begin + " */\n"
        ret += begin
        if not self.is_constructor() and not self.is_destructor():
            ret += self._p_return_type + " "
        ret += self.__name_() + "("
        ret += ", ".join(args) + ")"
        if template:
            ret += "\n" + begin + "{\n" + begin + "}\n"
        else:
            ret += ";\n"
        return ret


    #####################
    # PRIVATE - Methods #
    #####################


    def __name_(self):
        """
        Detailed description.
        """
        if self.isMethod():
            return self._p_name.replace("^",self.parent().parent()._p_name)
        else:
            return self._p_name


    def __checkFlags_(self):
        """
        Sets this function's flags to legal values if it is not a method.
        """
        if not self.isMethod():
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
        if self.isStatic():
            ret += "S"
        if self.is_noexcept():
            ret += "N"
        if self.is_virtual():
            ret += "V"
        if self.is_override():
            ret += "O"
        if self.is_final():
            ret += "F"
        if self.isAbstract():
            ret += "A"
        return ret


    def __argumentsView_(self):
        """
        Getter method.

        return : Rich text detailed view of all this function's arguments. If this function has no
                 arguments then this returns an empty string.
        """
        return ut.richText(
            2
            ,"Arguments"
            ,"".join((child.argumentView() for child in self if child._TYPE_ == "Variable"))
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
        if self.isStatic():
            flags.append("Static")
        if self.is_noexcept():
            flags.append("No Exceptions")
        if self.is_virtual():
            flags.append("Virtual")
        if self.is_override():
            flags.append("Override")
        if self.is_final():
            flags.append("Final")
        if self.isAbstract():
            flags.append("Abstract")
        return flags
