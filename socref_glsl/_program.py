"""
Contains the Program class.
"""
import html
from PySide2 import QtGui as qtg
from socref import abstract
from socref import edit
from socref import register
from . import parser








@register("Program",root=True)
class Program(abstract.AbstractBlock):
    """
    This is the program block class. It implements the Socrates' Reference
    abstract block class. It represents a GLSL program. It is the root block
    type of a GLSL project. A program simply represents a folder that contains
    shader files.
    """


    def __init__(
        self
        ):
        """
        Initializes a new program block.
        """
        super().__init__()
        self._p_name = ""


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
        return ("Program","Shader")


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        self._p_name = ""


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
        return self._p_name


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
        return "<h1>PROGRAM</h1><p>"+html.escape(self._p_name)+"</p>"


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
        return qtg.QIcon(":/glsl/program.svg")


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
        self._p_name = "program"
