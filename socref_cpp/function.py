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


    def hasTemplates(self):
        """
        Getter method.

        return : True if this block has any templates or false otherwise.
        """
        for child in self:
            if child._TYPE_ == "Template":
                return True
        return False


    def templateDeclaration(self):
        """
        Getter method.

        return : A string of code that is the template declaration of this block based off its child
                 template blocks or an empty string if it has no templates.
        """
        args = []
        for child in self:
            if child._TYPE_ == "Template":
                args.append(child.buildArgument())
        if args:
            return "template<%s>" % ",".join(args)
        else:
            return ""


    def templateScope(self):
        """
        Getter method.

        return : A string of code that is the template scope of this block based off its child
                 template blocks or an empty string if it has no templates.
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


    def _templatesName_(self):
        """
        Getter method.

        return : A string that is a decoration for a block's display name providing information
                 about its templates. If this block has no templates then an empty string is
                 returned.
        """
        return "<> " if self.hasTemplates() else ""


    def _templatesView_(self):
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
        self._p_returnType = ""
        self._p_returnDescription = ""
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


    def buildDeclaration(self, begin):
        """
        Implements the .namespace.Base interface.

        begin : See interface docs.

        return : See interface docs.
        """
        ret = [""]*settings.H2LINES
        ret += self.__buildComments_(begin)
        (before,after) = self.__buildFlagDeclarations_()
        ret += self.__buildDeclaration_("","",begin,";",before,after)
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
            not self.isDefault()
            and not self.isDeleted()
            and not self.isAbstract()
            and (
                (not header and not template and not self.hasTemplates())
                or (header and (template or self.hasTemplates()))
            )
        ):
            ret = [""]*settings.H1LINES
            (before,after) = self.__buildFlagDefinitions_()
            ret += self.__buildDeclaration_(scope,template,""," {}",before,after)
            return ([],ret)
        else:
            return ([],[])


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Template","Variable")


    def buildSource(self, definitions):
        """
        Implements the .namespace.Base interface.

        definitions : See interface docs.

        return : See interface docs.
        """
        ret = [""]*settings.H1LINES
        ret += self.__buildDeclaration_("","",""," {}","","")
        return ret


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.clearProperties(self)
        self._p_returnType = ""
        self._p_returnDescription = ""
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


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = self._templatesName_()
        if not self.isConstructor() and not self.isDestructor() and self._p_returnType != "void":
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
        if self._p_returnType != "void":
            return_ = "<p><b>%s</b> : %s</p>" % (self._p_returnType,self._p_returnDescription)
        return_ = ut.richText(2,"Return",return_)
        flags = ut.richTextList(2,"Flags",self.__flagsList_())
        return (
            namespace.Base.displayView(self)
            + self._templatesView_()
            + self.__argumentsView_()
            + return_
            + flags
        )


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = namespace.Base.editDefinitions(self)
        ret.append(ut.lineEdit("Return Type:","_p_returnType"))
        ret.append(ut.textEdit("Return Description:","_p_returnDescription",speller=True))
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


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        if self.isConstructor():
            return qtg.QIcon(":/cpp/constructor.svg")
        elif self.isDestructor():
            if self.isAbstract():
                return qtg.QIcon(":/cpp/abstract_destructor.svg")
            elif self.isVirtual():
                return qtg.QIcon(":/cpp/virtual_destructor.svg")
            else:
                return qtg.QIcon(":/cpp/destructor.svg")
        elif self.isOperator():
            if self.isAbstract():
                return qtg.QIcon(":/cpp/abstract_operator.svg")
            elif self.isVirtual():
                return qtg.QIcon(":/cpp/virtual_operator.svg")
            else:
                return qtg.QIcon(":/cpp/operator.svg")
        else:
            if self.isAbstract():
                return qtg.QIcon(":/cpp/abstract_function.svg")
            elif self.isVirtual():
                return qtg.QIcon(":/cpp/virtual_function.svg")
            elif self.isStatic():
                return qtg.QIcon(":/cpp/static_function.svg")
            else:
                return qtg.QIcon(":/cpp/function.svg")


    def isAbstract(self):
        """
        Getter method.

        return : True if this function is abstract or false otherwise.
        """
        return bool(int(self._p_abstract))


    def isConstant(self):
        """
        Getter method.

        return : True if this function is constant or false otherwise.
        """
        return bool(int(self._p_const))


    def isConstructor(self):
        """
        Getter method.

        return : True if this function is a constructor or false otherwise.
        """
        return self._p_name == "^"


    def isDefault(self):
        """
        Getter method.

        return : True if this function is the default or false otherwise.
        """
        return bool(int(self._p_default))


    def isDeleted(self):
        """
        Getter method.

        return : True if this function is deleted or false otherwise.
        """
        return bool(int(self._p_deleted))


    def isDestructor(self):
        """
        Getter method.

        return : True if this function is a destructor or false otherwise.
        """
        return self._p_name == "~^"


    def isExplicit(self):
        """
        Getter method.

        return : True if this function is an explicit constructor or false otherwise.
        """
        return bool(int(self._p_explicit))


    def isFinal(self):
        """
        Getter method.

        return : True if this function is the final overriding virtual interface or false otherwise.
        """
        return bool(int(self._p_final))


    def isMethod(self):
        """
        Getter method.

        return : True if this function is a method or false otherwise.
        """
        return self.parent()._TYPE_ == "Access"


    def isNoExcept(self):
        """
        Getter method.

        return : True if this function does not throw exceptions or false otherwise.
        """
        return bool(int(self._p_noexcept))


    def isOperator(self):
        """
        Getter method.

        return : True if this function is an operator or false otherwise.
        """
        return self._p_name.startswith("operator")


    def isOverride(self):
        """
        Getter method.

        return : True if this function is overriding a virtual interface or false otherwise.
        """
        return bool(int(self._p_override))


    def isStatic(self):
        """
        Getter method.

        return : True if this function is static or false otherwise.
        """
        return bool(int(self._p_static))


    def isVirtual(self):
        """
        Getter method.

        return : True if this function is virtual or false otherwise.
        """
        return bool(int(self._p_virtual))


    def isVolatileAbove(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return self.isMethod()


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.setDefaultProperties(self)
        self._p_name = "function"
        self._p_returnType = "void"
        self._p_returnDescription = "Detailed Description."
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


    def __buildComments_(self, begin):
        """
        Getter method.

        begin : A string that is added to the beginning of every returned line of code.

        return : A list of source code lines that is the block comment for this function.
        """
        ret = [begin+"/*!"]
        ret += ut.wrapBlocks(self._p_description,begin+" * ",begin+" *",settings.COLUMNS)
        for child in self:
            ret += [begin+" *"] + child.buildComment(begin+" * ")
        if self._p_returnType != "void":
            header = "@return : "
            ret.append(begin+" *")
            ret += ut.wrapText(header + self._p_returnDescription,begin+" * "," "*len(header),100)
        ret.append(begin+" */")
        return ret


    def __buildDeclaration_(self, scope, template, begin, end, beforeFlags, afterFlags):
        """
        Getter method.

        scope : The scope that is appended to the returned functions name.

        template : Any template declarations that is appended to its own line before the main
                   function declaration.

        begin : A string that is added to the beginning of every returned line of returned code.

        end : A string that is added to the end of the returned declaration.

        beforeFlags : The flags added to the returned declaration before the function return type
                      and name.

        afterFlags : The flags added to the returned declaration after the function name and
                     arguments.

        return : A list of lines that is the declaration, or header, of this function. If this
                 function has no arguments or templates then a single line is returned.
        """
        if scope:
            scope += "::"
        if self.hasTemplates():
            if template:
                template += " "
            template += self.templateDeclaration()
        ret = []
        if template:
            ret.append(begin+template)
        line = [begin+beforeFlags]
        if self._p_returnType and not self.isConstructor() and not self.isDestructor():
            line.append(self._p_returnType + " ")
        line.append(scope+self.__name_()+"(")
        args = []
        for child in self:
            if child._TYPE_ == "Variable":
                args.append(child.buildArgument())
        if not args:
            line.append(")"+afterFlags+end)
            ret.append("".join(line))
        else:
            newBegin = begin + " "*settings.INDENT
            ret.append("".join(line))
            first = True
            for arg in args:
                ret.append(newBegin+arg)
                if first:
                    newBegin = newBegin + ","
                    first = False
            ret.append(begin+")"+afterFlags+end)
        return ret


    def __buildFlagDeclarations_(self):
        """
        Getter method.

        return : A tuple containing two code fragment strings. The first are flags for this
                 function's declaration before its return type and name. The second are flags for
                 its declaration after its name and arguments.
        """
        before = ""
        after = ""
        if self.isDefault():
            after += " = default"
        if self.isDeleted():
            after += " = delete"
        if self.isExplicit():
            before += "explicit "
        if self.isConstant():
            after += " const"
        if self.isStatic():
            before += "static "
        if self.isNoExcept():
            after += " noexcept"
        if self.isVirtual():
            before += "virtual "
        if self.isOverride():
            after += " override"
        if self.isFinal():
            after += " final"
        if self.isAbstract():
            after += " = 0"
        return (before,after)


    def __buildFlagDefinitions_(self):
        """
        Getter method.

        return : A tuple containing two code fragment strings. The first are flags for this
                 function's definition header before its return type and name. The second are flags
                 for its definition header after its name and arguments.
        """
        ret = ""
        if self.isConstant():
            ret += " const"
        if self.isNoExcept():
            ret += " noexcept"
        return ("",ret)


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
        if self.isDefault():
            ret += "D"
        if self.isDeleted():
            ret += "R"
        if self.isExplicit():
            ret += "E"
        if self.isConstant():
            ret += "C"
        if self.isStatic():
            ret += "S"
        if self.isNoExcept():
            ret += "N"
        if self.isVirtual():
            ret += "V"
        if self.isOverride():
            ret += "O"
        if self.isFinal():
            ret += "F"
        if self.isAbstract():
            ret += "A"
        return ret


    def __flagsList_(self):
        """
        Getter method.

        return : A string list of flags this block has enabled. If this block has no flags enabled
                 then an empty string is returned.
        """
        flags = []
        if self.isDefault():
            flags.append("Default")
        if self.isDeleted():
            flags.append("Deleted")
        if self.isExplicit():
            flags.append("Explicit")
        if self.isConstant():
            flags.append("Constant")
        if self.isStatic():
            flags.append("Static")
        if self.isNoExcept():
            flags.append("No Exceptions")
        if self.isVirtual():
            flags.append("Virtual")
        if self.isOverride():
            flags.append("Override")
        if self.isFinal():
            flags.append("Final")
        if self.isAbstract():
            flags.append("Abstract")
        return flags


    def __name_(self):
        """
        Getter method.

        return : The name of this function, taking into account if it is a constructor or
                 destructor.
        """
        if self.isMethod():
            return self._p_name.replace("^",self.parent().parent()._p_name)
        else:
            return self._p_name
