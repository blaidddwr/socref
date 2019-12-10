"""
Contains the class block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import settings
from . import namespace
from . import function








@register("Class")
class Class(function.Templatee):
    """
    This is the class block class. It implements the Socrates' Reference abstract block class. It
    represents a C++ class.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new class block.
        """
        function.Templatee.__init__(self)
        self._p_header = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def buildDeclaration(self, begin, template):
        """
        Detailed description.

        begin : Detailed description.

        template : Detailed description.
        """
        if self.hasTemplates():
            template = True
        ret = [""]*settings.H1LINES
        ret.append(begin+"/*!")
        ret += ut.wrapBlocks(self._p_description,begin+" * ",begin+" *",settings.COLUMNS)
        ret.append(begin+" */")
        ret.append(begin+self.templateDeclaration()+"class "+self._p_name)
        ret.append(begin+"{")
        newBegin = begin + " "*settings.INDENT
        ret += [newBegin + line for line in self._p_header.split("\n") if line]
        for child in self:
            if child._TYPE_ == "Access":
                ret += child.buildHeader(begin,template)
        ret.append(begin+"};")
        return ret


    def buildDefinitions(self, begin, template):
        """
        Detailed description.

        begin : Detailed description.

        template : Detailed description.
        """
        pass


    def buildHeader(self, definitions):
        """
        Detailed description.

        definitions : Detailed description.
        """
        ret = self.__buildHeaderGuard_()
        names = []
        up = self.parent()
        while up is not None:
            if up._TYPE_ == "Namespace" and up.parent():
                names.append(up._p_name)
            up = up.parent()
        names.reverse()
        if names:
            ret.append("")
            for name in names:
                ret += ["namespace "+name,"{"]
        ret += self.buildDeclaration("",False)
        defs = self.buildHeaderDefinitions("",False)
        if defs:
            ret += [""]*settings.H2LINES + defs
        if names:
            ret += [""] + ["}"]*len(names)
        ret.append("#endif")
        return ret


    def buildHeaderDefinitions(self, scope, template):
        """
        Detailed description.

        scope : Detailed description.

        template : Detailed description.
        """
        if scope:
            scope += "::"
        scope += self._p_name
        if self.hasTemplates():
            template = True
            scope += self.templateScope()
        ret = []
        for child in self:
            if child._TYPE_ == "Access":
                ret += child.buildHeaderDefinitions(scope,template)
        return ret


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Template","Access")


    def buildSource(self):
        """
        Detailed description.
        """
        pass


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.clearProperties(self)
        self._p_header = ""


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return self._templatesName_() + self._p_name


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return namespace.Base.displayView(self) + self._templatesView_()


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = namespace.Base.editDefinitions(self)
        ret.append(ut.textEdit("Header:","_p_header"))
        return ret


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        if self.isAbstract():
            return qtg.QIcon(":/cpp/abstract_class.svg")
        elif self.isVirtual():
            return qtg.QIcon(":/cpp/virtual_class.svg")
        else:
            return qtg.QIcon(":/cpp/class.svg")


    def isAbstract(self):
        """
        Getter method.

        return : True if this class contains any abstract functions or false otherwise.
        """
        for child in self:
            if child._TYPE_ == "Access" and child.hasAbstract():
                return True
        return False


    def isVirtual(self):
        """
        Detailed description.
        """
        for child in self:
            if child._TYPE_ == "Access" and child.hasVirtual():
                return True
        return False


    def isVolatileBelow(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return True


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.setDefaultProperties(self)
        self._p_name = "class"
        self._p_header = ""


    #####################
    # PRIVATE - Methods #
    #####################


    def __buildHeaderGuard_(self):
        """
        Detailed description.
        """
        names = [self._p_name.upper()]
        up = self.parent()
        while up is not None:
            if (
                up._TYPE_ == "Class"
                or (up._TYPE_ == "Namespace" and up.parent() and not up.isHidden())
            ):
                names.append(up._p_name.upper())
            up = up.parent()
        names.reverse()
        guard = "_".join(names) + "_H"
        return ["#ifndef " + guard,"#define " + guard]
