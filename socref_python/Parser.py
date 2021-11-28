"""
Contains the Parser class.
"""
from .Reader.ModuleReader import ModuleReader
from .Writer.ModuleWriter import ModuleWriter
from os.path import join as pathJoin
from socref import parser
from socref.Base.ParserBase import ParserBase




@parser
class Parser(ParserBase):
    """
    This is the parser class. It implements the Socrates' Reference abstract
    parser class for the python language.
    """


    def __init__(
        self
        ,root
    ):
        """
        Initializes this new parser with the given root block.

        Parameters
        ----------
        root : PackageBlock
               The root block.
        """
        super().__init__()
        self.__root = root


    def _pathList_(
        self
    ):
        def build(path,parent):
            ret = []
            for block in parent:
                if block._TYPE_ == "Module":
                    ret.append(
                        (
                            pathJoin(path,block._p_name+".py")
                            ,ModuleReader(block,self)
                            ,ModuleWriter(block,self)
                        )
                    )
                    ret += build(path,block)
                elif block._TYPE_ == "Package":
                    ret.append(
                        (
                            pathJoin(path,block._p_name,"__init__.py")
                            ,ModuleReader(block,self)
                            ,ModuleWriter(block,self)
                        )
                    )
                    ret += build(pathJoin(path,block._p_name),block)
                elif block._TYPE_ == "Class" and not block.isInfile():
                    ret.append(
                        (
                            pathJoin(path,block._p_name+".py")
                            ,ModuleReader(block,self)
                            ,ModuleWriter(block,self)
                        )
                    )
            return ret
        return build("",self.__root)
