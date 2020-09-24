"""
Contains the Class class.
"""
from PySide2 import QtGui as qtg
from socref import edit
from socref import register
from . import block
from . import settings
from ._templatee import Templatee








@register("Class")
class Class(Templatee):
    """
    This is the class block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ class.
    """


    def __init__(
        self
        ):
        """
        Initializes a new class block.
        """
        super().__init__()
        self._p_parents = ""
        self._p_header = ""


    def buildDeclaration(
        self
        ,begin
        ):
        """
        Implements the socref_cpp.block.Base interface.

        Parameters
        ----------
        begin : object
                See interface docs.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = [""]*settings.H1LINES
        ret.append(begin+"/*!")
        ret += edit.wrapBlocks(self._p_description,begin+" * ",begin+" *",settings.COLUMNS)
        accesses = []
        for child in self:
            if child._TYPE_ == "Template":
                ret += [begin+" *"] + child.buildComment(begin+" * ")
            elif child._TYPE_ == "Access":
                accesses.append(child)
        ret.append(begin+" */")
        template = self.templateDeclaration()
        if template:
            ret.append(begin+template)
        ret.append(begin+"class "+self._p_name)
        ret += self.__buildParents_(begin)
        ret.append(begin+"{")
        ret += self._buildClassHeader_(begin + " "*settings.INDENT)
        for access in accesses:
                ret += access.buildDeclaration(begin)
        ret.append(begin+"};")
        return ret


    def buildDefinition(
        self
        ,definitions
        ,scope
        ,template
        ,header
        ):
        """
        Implements the socref_cpp.block.Base interface.

        Parameters
        ----------
        definitions : object
                      See interface docs.
        scope : object
                See interface docs.
        template : object
                   See interface docs.
        header : object
                 See interface docs.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        if scope:
            scope += "::"
        scope += self._p_name
        if self.hasTemplates():
            if template:
                template += " "
            template += self.templateDefinition()
            scope += self.templateScope()
        variables = []
        functions = []
        for child in self:
            if child._TYPE_ == "Access":
                (v,f) = child.buildDefinition(definitions,scope,template,header)
                variables += v
                functions += f
        return (variables,functions)


    def buildForward(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : list
               A single line that is the forward declaration of this class or an
               empty list if this class has any templates.
        """
        if not self.hasTemplates():
            return ["class "+self._p_name+";"]
        else:
            return []


    def buildHeader(
        self
        ,definitions
        ,path
        ):
        """
        Implements the socref_cpp.block.Base interface.

        Parameters
        ----------
        definitions : object
                      See interface docs.
        path : object
               See interface docs.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return block.Namespace.buildHeader(self,definitions,path)


    def buildList(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return ("Template","Access")


    def buildSource(
        self
        ,definitions
        ,path
        ):
        """
        Implements the socref_cpp.block.Base interface.

        Parameters
        ----------
        definitions : object
                      See interface docs.
        path : object
               See interface docs.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return block.Namespace.buildSource(self,definitions,path)


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        block.Base.clearProperties(self)
        self._p_parents = ""
        self._p_header = ""


    def displayName(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return self._p_name + self._templatesName_()


    def displayView(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        parents = edit.richText(
            2
            ,"Parents"
            ,"".join(("<li>%s</li>" % parent for parent in self._p_parents.split("\n") if parent))
        )
        return block.Base.displayView(self)+self._templatesView_()+parents


    def editDefinitions(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = block.Base.editDefinitions(self)
        ret.append(edit.textEdit("Parents:","_p_parents"))
        ret.append(edit.textEdit("Header:","_p_header"))
        return ret


    def icon(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        if self.isAbstract():
            return qtg.QIcon(":/cpp/abstract_class.svg")
        elif self.isVirtual():
            return qtg.QIcon(":/cpp/virtual_class.svg")
        else:
            return qtg.QIcon(":/cpp/class.svg")


    def isAbstract(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this class contains any abstract functions or false
               otherwise.
        """
        for child in self:
            if child._TYPE_ == "Access" and child.hasAbstract():
                return True
        return False


    def isVirtual(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this class contains any virtual functions or false
               otherwise.
        """
        for child in self:
            if child._TYPE_ == "Access" and child.hasVirtual():
                return True
        return False


    def isVolatileBelow(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return True


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        block.Base.setDefaultProperties(self)
        self._p_name = "class"
        self._p_parents = ""
        self._p_header = ""


    def _buildClassHeader_(
        self
        ,begin
        ):
        """
        Getter method.

        Parameters
        ----------
        begin : string
                Added to the beginning of each returned line of code.

        Returns
        -------
        ret0 : list
               Class header code lines that are added to this classes built
               declaration just after the initial opening bracket.
        """
        return [begin + line for line in self._p_header.split("\n") if line]


    def __buildParents_(
        self
        ,begin
        ):
        """
        Getter method.

        Parameters
        ----------
        begin : string
                Added to the beginning of each returned line of code.

        Returns
        -------
        ret0 : list
               Code lines, beginning with a colon, that are the parent
               declarations for this class. If this class has no parents then an
               empty list is returned.
        """
        ret = []
        parents = [parent for parent in self._p_parents.split("\n") if parent]
        if parents:
            ret.append(begin+":")
            begin += " "*settings.INDENT
            first = True
            for parent in parents:
                ret.append(begin+parent)
                if first:
                    begin += ","
                    first = False
        return ret
