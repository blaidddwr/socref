"""
Contains the Parser class.
"""
from .Reader.HeadReader import HeadReader
from .Reader.SourceReader import SourceReader
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
            for b in parent:
                if b._TYPE_ == "Namespace":
                    nPath = pathJoin(path,b._p_name)
                    ret.append((pathJoin(nPath,"__init__.h"),HeadReader(b,self),HeadWriter(b,self)))
                    if b.hasFunctions():
                        ret.append(
                            (
                                pathJoin(nPath,"__init__.cpp")
                                ,SourceReader(b,self)
                                ,SourceWriter(b,self)
                            )
                        )
                    ret += build(nPath,b)
                elif b._TYPE_ == "Class":
                    ret.append(
                        (pathJoin(path,b._p_name+".h"),HeadReader(b,self),HeadWriter(b,self))
                    )
                    ret.append(
                        (pathJoin(path,b._p_name+".cpp"),SourceReader(b,self),SourceWriter(b,self))
                    )
            return ret
        ret = []
        b = self.__root
        ret.append(("__init__.h",HeadReader(b,self),HeadWriter(b,self)))
        if b.hasFunctions():
            ret.append(("__init__.cpp",SourceReader(b,self),SourceWriter(b,self)))
        ret += build("",b)
        return ret
