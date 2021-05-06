"""
Contains the Parser class.
"""
from socref import parser
from socref.Abstract.AbstractParser import *




@parser
class Parser(AbstractParser):
    """
    This is the parser class. It implements the Socrates' Reference abstract
    parser class for the GLSL language.
    """


    def __init__(
        self
        ,root
    ):
        """
        Initializes this new parser with the given root block.

        Parameters
        ----------
        root : socref_glsl.Block.ProgramBlock
               The root block of the project this parser will use for generating
               paths, readers, and writers.
        """
        pass


    def _pathList_(
        self
    ):
        return []


    def _reader_(
        self
        ,block
    ):
        pass


    def _writer_(
        self
        ,block
    ):
        pass
