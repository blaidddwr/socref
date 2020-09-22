"""
Contains the Namespace class.
"""
from PySide2 import QtGui as qtg
from socref import register
from socref import edit
from . import parser
from . import settings
from ._base import Base








@register("Namespace",root=True)
class Namespace(Base):
    """
    This is the namespace block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ namespace. It is the root block
    type of a C++ project.
    """


    def __init__(
        self
        ):
        """
        Initializes a new namespace block.
        """
        Base.__init__(self)
        self._p_hidden = "0"


    def buildDeclaration(
        self
        ,begin
        ):
        """
        Implements the .namespace.Base interface.

        Parameters
        ----------
        begin : object
                See interface docs.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        forwards = []
        ret = []
        for child in self:
            if child._TYPE_ == "Class":
                forwards += child.buildForward()
            else:
                ret += child.buildDeclaration(begin)
        ret = forwards+ret
        return ret


    def buildDefinition(
        self
        ,definitions
        ,scope
        ,template
        ,header
        ):
        """
        Implements the .namespace.Base interface.

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
        variables = []
        functions = []
        for child in self:
            if child._TYPE_ != "Class":
                (v,f) = child.buildDefinition(definitions,scope,template,header)
                variables += v
                functions += f
        return (variables,functions)


    def buildHeader(
        self
        ,definitions
        ,path
        ):
        """
        Implements the .namespace.Base interface.

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
        ret = self._buildHeaderGuard_()
        ret += definitions["headers"].get(path,[])
        body = self.buildDeclaration("")
        (variables,functions) = self.buildDefinition(definitions,"","",True)
        if variables:
            body += [""]*settings.H1LINES
        body += variables+functions
        ret += self._buildNamespaces_(body,self._TYPE_ == "Namespace")
        ret += ["","#endif"]
        return ret


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
        return ("Namespace","Enumeration","Variable","Function","Class","Union")


    def buildSource(
        self
        ,definitions
        ,path
        ):
        """
        Implements the .namespace.Base interface.

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
        ret = definitions["headers"].get(path,[])
        body = []
        (variables,functions) = self.buildDefinition(definitions,"","",False)
        if variables:
            body += [""]*settings.H1LINES
        body += variables+functions
        ret += self._buildNamespaces_(body)
        return ret


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.clearProperties(self)
        self._p_hidden = "0"


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
        ret = Base.editDefinitions(self)
        ret.append(edit.checkboxEdit("Hidden","_p_hidden"))
        return ret


    def hasFunctions(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this namespace has one or more functions or false
               otherwise.
        """
        for child in self:
            if child._TYPE_ == "Function":
                return True
        return False


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
        if self.isHidden():
            return qtg.QIcon(":/cpp/hidden_namespace.svg")
        else:
            return qtg.QIcon(":/cpp/namespace.svg")


    def isHidden(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this is a hidden namespace that must not have its name
               added to the scoping of source code files nor have its own header
               or source file. If this is a regular visible namespace then false
               is returned.
        """
        return bool(int(self._p_hidden))


    def parser(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return parser.Parser(self)


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.setDefaultProperties(self)
        self._p_name = "namespace"
        self._p_hidden = "0"
