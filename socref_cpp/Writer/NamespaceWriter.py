"""
Contains the NamespaceWriter class.
"""
from ..Settings import Settings
from .EnumerationWriter import EnumerationWriter
from .FunctionWriter import FunctionWriter
from .UnionWriter import UnionWriter
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class NamespaceWriter(WriterBase):
    """
    This is the name space writer class. It implements the Socrates' Reference
    abstract writer class. It outputs all code lines for a name space
    declaration.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new name space writer and its children writers with the
        given name space block and parent head writer. The given name space
        block is used by this writer to find its corresponding reader,
        initialize its children writers, and generate its output.

        Parameters
        ----------
        block : NamespaceBlock
                The namespace block.
        parent : HeadWriter
                 The parent head writer.
        """
        super().__init__(parent)
        self.__block = block
        depth = 1 if block.parent() else 0
        for child in block:
            if child._TYPE_ == "Function":
                FunctionWriter(child,depth,self)
            elif child._TYPE_ == "Enumeration":
                EnumerationWriter(child,depth,self)
            elif child._TYPE_ == "Union":
                UnionWriter(child,depth,self)


    def _footer_(
        self
    ):
        ret = Code(Settings.INDENT)
        if self.__block.parent():
            ret.add("}")
        return ret


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        if self.__block.parent():
            ret.addBlank(Settings.H2)
            ret.add("/*!")
            ret.addText(self.__block._p_description,Settings.COLS," * ")
            ret.add(" */")
            ret.add("namespace "+self.__block._p_name)
            ret.add("{")
            ret.setDepth(1)
        for child in self.__block:
            if child._TYPE_ == "Class":
                ret.add("class "+child._p_name+";")
        return ret
