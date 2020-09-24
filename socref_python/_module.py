"""
Contains the Module class.
"""
from PySide2 import QtGui as qtg
from socref import register
from ._package import Package
from . import settings








@register("Module")
class Module(Package):
    """
    This is the module block class. It implements the Socrates' Reference
    abstract block class. It represents a python module.
    """


    def __init__(
        self
        ):
        """
        Initializes a new module block.
        """
        super().__init__()


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
        (header,footer) = Package._build_(self,definition,begin)
        ret = header
        (regular,classes) = self._buildChildren_(definition,begin)
        if classes:
            ret += [""]+classes
        ret += regular
        if self._p_name == "__main__":
            ret += [""]*settings.H1LINES
            ret.append('if __name__ == "__main__": main()')
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
        return ("Object","Function","Class")


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
        return qtg.QIcon(":/python/module.svg")


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Package.setDefaultProperties(self)
        self._p_name = "module"
