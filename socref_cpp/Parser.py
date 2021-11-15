"""
Contains the Parser class.
"""
from socref import parser
from socref.Base.ParserBase import ParserBase




@parser
class Parser(ParserBase):
    """
    This is the parser class. It implements the Socrates' Reference abstract
    parser class for the C++ language.
    """


    def __init__(
        self
        ,root
    ):
        """
        Initializes this new parser with the given root block.

        Parameters
        ----------
        root : NamespaceBlock
               The root block.
        """
        super().__init__()
        self.__root = root


    def _pathList_(
        self
    ):
        return []
