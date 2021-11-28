"""
Contains the Parser class.
"""
from .Reader.HeadReader import HeadReader
from .Reader.SourceReader import SourceReader
from .Settings import Settings
from .Writer.HeadWriter import HeadWriter
from .Writer.SourceWriter import SourceWriter
from os.path import join as pathJoin
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
        def build(path,parent):
            ret = []
            if parent:
                ret.append(
                    (
                        pathJoin(path,Settings.NS_NAME+Settings.HDR_EXT)
                        ,HeadReader(parent,self)
                        ,HeadWriter(parent,self))
                )
                if parent.hasFunctions():
                    ret.append(
                        (
                            pathJoin(path,Settings.NS_NAME+Settings.SRC_EXT)
                            ,SourceReader(parent,self)
                            ,SourceWriter(parent,self)
                        )
                    )
            for child in parent:
                if child._TYPE_ == "Namespace":
                    ret += build(pathJoin(path,child._p_name),child)
                elif child._TYPE_ == "Class":
                    ret.append(
                        (
                            pathJoin(path,child._p_name+Settings.HDR_EXT)
                            ,HeadReader(child,self)
                            ,HeadWriter(child,self)
                        )
                    )
                    ret.append(
                        (
                            pathJoin(path,child._p_name+Settings.SRC_EXT)
                            ,SourceReader(child,self)
                            ,SourceWriter(child,self)
                        )
                    )
            return ret
        return build("",self.__root)
