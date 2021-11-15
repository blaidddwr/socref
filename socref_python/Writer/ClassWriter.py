"""
Contains the ClassWriter class.
"""
from ..Settings import Settings
from .FunctionWriter import FunctionWriter
from .ObjectWriter import ObjectWriter
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class ClassWriter(WriterBase):
    """
    This is the class writer class. It implements the Socrates' Reference
    abstract writer class. It outputs the header code lines for a class.
    Children writers for methods, objects, and nested classes output their code
    lines after the header.
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
        corresponding reader, initialize its children writers, and generate
        output. The given parent writer must be a module or class writer.

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
            if child._TYPE_ == "Function":
                FunctionWriter(child,depth+1,self)
            elif child._TYPE_ == "Object":
                ObjectWriter(child,depth+1,self)
            elif child._TYPE_ == "Class":
                ClassWriter(child,depth+1,self)


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        if self.__block.parent()._TYPE_ == "Class":
            ret.addBlank(Settings.H2)
        else:
            ret.addBlank(Settings.H1)
        ret.setDepth(self.__depth)
        ret.add(self.__block.decorators())
        ret.add("class "+self.__block._p_name+"("+",".join(self.__block.clsParents())+"):")
        ret.setDepth(self.__depth+1)
        ret.add('"""')
        ret.addText(self.__block._p_description,Settings.COLS)
        ret.add('"""')
        if self.__reader:
            ret.add(self.__reader.lines())
        elif len(self.__block) == 0:
            ret.add("pass")
        return ret


    def _link_(
        self
    ):
        self.__reader = self.lookup(self.__block.key())
