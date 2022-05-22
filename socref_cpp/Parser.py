"""
Contains the Parser class.
"""
from .Reader.HeadReader import HeadReader
from .Reader.SourceReader import SourceReader
from .Settings import Settings
from .Writer.HeadWriter import HeadWriter
from .Writer.SourceWriter import SourceWriter
from os.path import join as pathJoin
from re import sub
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
        def build(parent):
            ret = []
            ret.append(
                (parent.key()+Settings.HDR_EXT,HeadReader(parent,self),HeadWriter(parent,self))
            )
            if parent.hasFunctions():
                ret.append(
                    (
                        parent.key()+Settings.SRC_EXT
                        ,SourceReader(parent,self)
                        ,SourceWriter(parent,self)
                    )
                )
            for child in parent:
                if child._TYPE_ == "Namespace":
                    ret += build(child)
                elif child._TYPE_ == "Class":
                    ret.append(
                        (
                            child.key()+Settings.HDR_EXT
                            ,HeadReader(child,self)
                            ,HeadWriter(child,self)
                        )
                    )
                    ret.append(
                        (
                            child.key()+Settings.SRC_EXT
                            ,SourceReader(child,self)
                            ,SourceWriter(child,self)
                        )
                    )
            return ret
        return build(self.__root)
