"""
Contains the Structure class.
"""
import html
from PySide2 import QtGui as qtg
from socref import edit
from socref import register
from . import settings
from ._base import Base








@register("Structure")
class Structure(Base):
    """
    This is the structure block class. It implements the Socrates' Reference
    abstract block class. It represents a GLSL structure.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(
        self
        ):
        """
        Initializes a new structure block.
        """
        Base.__init__(self)
        self._p_type = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def build(
        self
        ,definition
        ,begin=""
        ):
        """
        Implements the socref_glsl.block.Base interface.

        Parameters
        ----------
        definition : object
                     See interface docs.
        begin : object
                See interface docs.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = [""]*settings.H2LINES
        ret.append(begin+"/*!")
        ret += edit.wrapText(self._p_description,begin+" * ",columns=settings.COLUMNS)
        ret.append(begin+" */")
        ret.append(begin+self._p_type)
        ret.append("{")
        nextBegin = begin + " "*settings.INDENT
        ret += self._buildChildren_(definition,nextBegin)
        line = "}"
        if self._p_name:
            line += " %s;"%self._p_name
        else:
            line += ";"
        ret.append(line)
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
        return ("Variable",)


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.clearProperties(self)
        self._p_type = ""


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
        if self._p_name:
            return self._p_name
        else:
            return self._p_type.split()[-1]


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
        type_ = "<h2>Type</h2><p>"+html.escape(self._p_type)+"</p>"
        return Base.displayView(self)+type_


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
        ret.append(edit.lineEdit("Type:","_p_type"))
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
        return qtg.QIcon(":/glsl/structure.svg")


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.setDefaultProperties(self)
        self._p_name = "structure"
        self._p_type = "struct Struct"
