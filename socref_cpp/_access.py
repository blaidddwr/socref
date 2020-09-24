"""
Contains the Access class.
"""
import html
from PySide2 import QtGui as qtg
from socref import abstract
from socref import edit
from socref import register
from . import settings








@register("Access")
class Access(abstract.AbstractBlock):
    """
    This is the access block class. It implements the Socrates' Reference
    abstract block class. It represents an access declaration for C++ class.
    """


    def __init__(
        self
        ):
        """
        Initializes a new access block.
        """
        super().__init__()
        self._p_name = ""
        self._p_type = "Public"
        self._p_enclosure = ""


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
        ret = [""]*settings.H2LINES
        header = []
        footer = []
        nextBegin = begin + " "*settings.INDENT
        if self._p_enclosure:
            blocks = self._p_enclosure.split("\n@\n")
            if blocks:
                header = [nextBegin+line for line in blocks[0].split("\n") if line]
            if len(blocks) > 1:
                footer = [nextBegin+line for line in blocks[1].split("\n") if line]
        decoration = begin + "/**" + "*"*len(self._p_name) + "**/"
        ret.append(decoration)
        ret.append(begin + "/* %s */"%self._p_name)
        ret.append(decoration)
        ret.append(begin+self._p_type.lower()+":")
        ret += header
        for child in self:
            ret += child.buildDeclaration(nextBegin)
        if footer:
            ret += [""]*settings.H2LINES
        ret += footer
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
        return ("Enumeration","Variable","Function","Class","Union")


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
        variables = []
        functions = []
        for child in self:
            (v,f) = child.buildDefinition(definitions,scope,template,header)
            variables += v
            functions += f
        return (variables,functions)


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        self._p_name = ""
        self._p_type = "Public"
        self._p_enclosure = ""


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
        return self._p_name + " (%i)" % len(self)


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
        return "<h1>%s</h1><p>%s</p>" % (
            html.escape(self._p_type.upper())
            ,html.escape(self._p_name)
        )


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
        ret = []
        ret.append(edit.lineEdit("Name:","_p_name"))
        combo = edit.comboEdit("Type:","_p_type")
        self._addComboSelects_(combo)
        ret.append(combo)
        ret.append(edit.textEdit("Enclosure:","_p_enclosure"))
        return ret


    def hasAbstract(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this access block contains any abstract methods or false
               otherwise.
        """
        for block in self:
            if block._TYPE_ == "Function":
                if block.isAbstract():
                    return True
        return False


    def hasVirtual(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this access block contains any virtual methods or false
               otherwise.
        """
        for block in self:
            if block._TYPE_ == "Function":
                if block.isVirtual():
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
        if self._p_type == "Public":
            return qtg.QIcon(":/cpp/public.svg")
        elif  self._p_type == "Protected":
            return qtg.QIcon(":/cpp/protected.svg")
        else:
            return qtg.QIcon(":/cpp/private.svg")


    def isVolatileAbove(
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
        self._p_name = "access"
        self._p_type = "Public"
        self._p_enclosure = ""


    def _addComboSelects_(
        self
        ,combo
        ):
        """
        Adds the selection values available for modifying this access block's
        type to the given combo edit definition.

        Parameters
        ----------
        combo : PySide2.QtWidgets.QComboBox
                The combo box that is populated with possible access type
                values.
        """
        edit.addComboSelect(combo,"Public",icon=qtg.QIcon(":/cpp/public.svg"))
        edit.addComboSelect(combo,"Protected",icon=qtg.QIcon(":/cpp/protected.svg"))
        edit.addComboSelect(combo,"Private",icon=qtg.QIcon(":/cpp/private.svg"))
