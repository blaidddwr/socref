"""
Contains the Access class.
"""
import html
from PySide2 import QtGui as qtg
from socref import abstract
from socref import edit
from socref import register
from ._package import Package
from . import settings








@register("Access")
class Access(Package):
    """
    This is the access block class. It implements the Socrates' Reference
    abstract block class. It represents a fictitious access declaration for
    python classes. Since python does not actually have any kind of access
    control, this is simply displayed as a comment inside of the class source
    code. It is mostly helpful in the reference application's project tree view
    for organizing different parts of a class.
    """


    def __init__(
        self
        ):
        """
        Initializes a new access block.
        """
        abstract.AbstractBlock.__init__(self)
        self._p_name = ""
        self._p_type = "Public"


    def build(
        self
        ,definition
        ,begin=""
        ):
        """
        Implements the socref_python.block.Package interface.

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
        ret = [""]*settings.H3LINES
        line = "# %s - %s #" % (self._p_type.upper(),self._p_name)
        ret.append(begin + "#"*len(line))
        ret.append(begin + line)
        ret.append(ret[-2])
        (regular,classes) = self._buildChildren_(definition,begin)
        ret += regular
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
        return ("Object","Function")


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        self._p_name = ""
        self._p_type = "Public"


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
        edit.addComboSelect(combo,"Public",icon=qtg.QIcon(":/python/public.svg"))
        edit.addComboSelect(combo,"Protected",icon=qtg.QIcon(":/python/protected.svg"))
        edit.addComboSelect(combo,"Private",icon=qtg.QIcon(":/python/private.svg"))
        ret.append(combo)
        return ret


    def hasAbstract(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this access block contains abstract methods or false
               otherwise.
        """
        for block in self:
            if block._TYPE_ == "Function":
                if block.isAbstract():
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
            return qtg.QIcon(":/python/public.svg")
        elif  self._p_type == "Protected":
            return qtg.QIcon(":/python/protected.svg")
        else:
            return qtg.QIcon(":/python/private.svg")


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


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        self._p_name = "DEPRECATED"
        self._p_type = "Public"
