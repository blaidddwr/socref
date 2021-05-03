"""
Contains the PackageBlock class.
"""
from .ModuleBlock import *
from PySide2.QtGui import QIcon
from socref import block




@block("Package",root=True)
class PackageBlock(ModuleBlock):
    """
    This is the package block class. It implements the Socrates' Reference
    abstract block class. It represents a python package.
    """


    def buildList(
        self
    ):
        if self.parent() is None:
            return ("Package","Module","Class")
        else:
            return ("Package","Module","Function","Class")


    def icon(
        self
    ):
        return QIcon(":/socref_python/package.svg")


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "package"
