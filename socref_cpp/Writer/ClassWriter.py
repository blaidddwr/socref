"""
Contains the ClassWriter class.
"""
from ..Settings import Settings
from .EnumerationWriter import EnumerationWriter
from .FunctionWriter import FunctionWriter
from .UnionWriter import UnionWriter
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class ClassWriter(WriterBase):
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
        super().__init__(parent)
        self.__block = block
        self.__depth = depth
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


    def _footer_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.setDepth(self.__depth)
        ret.add("};")
        return ret


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.setDepth(self.__depth)
        if self.__block.parent()._TYPE_ == "Namespace":
            ret.addBlank(Settings.H1)
        else:
            ret.addBlank(Settings.H2)
        ret.add("/**")
        ret.addText(self.__block._p_description,Settings.COLS," * ")
        ret.add(" */")
        ret.add(
            ("public: " if self.__block.parent()._TYPE_ == "Class" else "")
            + "class "
            + self.__block._p_name
        )
        ret.add("{")
        if self.__reader:
            ret.setDepth(self.__depth+1)
            ret.add(self.__reader.lines())
        return ret


    def _link_(
        self
    ):
        self.__reader = self.lookup(self.__block.key())
