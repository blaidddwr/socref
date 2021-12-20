"""
Contains the ClassWriter class.
"""
from ..Base.BlockWriterBase import BlockWriterBase
from .EnumerationWriter import EnumerationWriter
from .FunctionWriter import FunctionWriter
from .UnionWriter import UnionWriter




class ClassWriter(BlockWriterBase):
    """
    This is the class writer class. It implements the Socrates' Reference
    abstract writer class. It outputs all code lines for a class declaration.
    """


    def __init__(
        self
        ,block
        ,depth
        ,parent
    ):
        """
        Initializes this new class writer and its children writers with the
        given class block, indentation depth of this writer's output, and parent
        writer. The given class block is used by this writer to find its
        corresponding reader, initialize its children writers, and generate its
        output. The given parent writer must be a header or class writer.

        Parameters
        ----------
        block : ClassBlock
                The class block.
        depth : int
                The indentation depth.
        parent : AbstractWriter
                 The parent writer.
        """
        super().__init__(block,depth,parent)
        self.__block = block
        self.__reader = None
        for child in block:
            if child._TYPE_ == "Class":
                ClassWriter(child,depth+1,self)
            elif child._TYPE_ == "Function":
                FunctionWriter(child,depth+1,self)
            elif child._TYPE_ == "Enumeration":
                EnumerationWriter(child,depth+1,self)
            elif child._TYPE_ == "Union":
                UnionWriter(child,depth+1,self)


    def _lines_(
        self
    ):
        return self.__reader.lines() if self.__reader else []


    def _link_(
        self
    ):
        self.__reader = self.lookup(self._block_().key())


    def _name_(
        self
    ):
        return "class"


    def _preLines_(
        self
    ):
        ret = []
        comma = ""
        for parent in self.__block.parents():
            ret.append(comma+parent)
            if not comma:
                comma = ","
        return ret
