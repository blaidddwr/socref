"""
Contains the ClassWriter class.
"""
from .FunctionWriter import *
from .ObjectWriter import *
from socref.Abstract.AbstractWriter import *
from socref.Output.Code import *




class ClassWriter(AbstractWriter):
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
        Initializes this new class writer with the given class block, indent
        depth, and parent writer.

        Parameters
        ----------
        block : socref_python.Block.ClassBlock
                The class block this writer uses to find its corresponding
                reader, construct its children writers, and generate output.
        depth : int
                The indentation depth of this writer's output.
        parent : socref.Abstract.AbstractWriter
                 The parent writer of this writer. This must be a module or
                 class writer.
        """
        super().__init__(parent)
        self.__block = block
        self.__depth = depth
        self.__reader = self.lookup(block.key())
        for child in block:
            if child._TYPE_ == "Function":
                FunctionWriter(child,depth+1,self)
            elif child._TYPE_ == "Object":
                ObjectWriter(child,depth+1,self)
            elif child._TYPE_ == "Class":
                ClassWriter(child,depth+1,self)


    def _footer_(
        self
    ):
        return []


    def _header_(
        self
    ):
        ret = Code("    ")
        if self.__block.parent()._TYPE_ == "Class":
            ret.addBlank(2)
        else:
            ret.addBlank(4)
        ret.setDepth(self.__depth)
        ret.add(self.__block.decorators())
        ret.add("class "+self.__block._p_name+"("+",".join(self.__block.clsParents())+"):")
        ret.setDepth(self.__depth+1)
        ret.add('"""')
        ret.addText(self.__block._p_description,80)
        ret.add('"""')
        if self.__reader:
            ret.add(self.__reader.lines())
        elif len(self.__block) == 0:
            ret.add("pass")
        return ret
