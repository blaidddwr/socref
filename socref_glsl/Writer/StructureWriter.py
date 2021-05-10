"""
Contains the StructureWriter class.
"""
from ..Settings import *
from .VariableWriter import *
from socref.Abstract.AbstractWriter import *
from socref.Output.Code import *




class StructureWriter(AbstractWriter):
    """
    This is the structure writer class. It implements the Socrates' Reference
    abstract writer class. It outputs the header and footer code lines for a
    structure. Children writers for nested variables output their code after the
    header.
    """


    def __init__(
        self
        ,block
        ,depth
        ,parent
    ):
        """
        Initializes this new structure writer and its children writers with the
        given block and parent writer.

        Parameters
        ----------
        block : socref_glsl.Block.VariableBlock
                The structure block this writer uses to initialize its children
                writers and generate its output.
        depth : int
                The indentation depth of this writer's output.
        parent : socref.Abstract.AbstractWriter
                 The parent writer of this writer. This must be a shader writer.
        """
        super().__init__(parent)
        self.__block = block
        self.__depth = depth
        for child in block:
            if child._TYPE_ == "Variable":
                VariableWriter(child,depth+1,self)


    def _footer_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.setDepth(self.__depth)
        if self.__block._p_name:
            ret.add("} "+self.__block._p_name+";")
        else:
            ret.add("};")
        return ret


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.addBlank(Settings.H2)
        ret.setDepth(self.__depth)
        ret.add("/*!")
        ret.addText(self.__block._p_description,Settings.COLS," * ")
        ret.add(" */")
        ret.add(self.__block._p_type)
        ret.add("{")
        return ret
