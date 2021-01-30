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
    abstract block class. It represents a python package. It is the root block
    type of a python project.
    """


    def buildList(
        self
    ):
        if self.parent() is None:
            #return ("Package","Module","Class")
            return ("Package","Module")
        else:
            #return ("Package","Module","Function","Class")
            return ("Package","Module","Function")


    def icon(
        self
    ):
        return QIcon(":/python/package.svg")


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "package"
