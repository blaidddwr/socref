"""
Contains the VariableWriter class.
"""
from ..Settings import *
from socref.Base.WriterBase import *
from socref.Output.Code import *




class VariableWriter(WriterBase):
    """
    This is the variable writer class. It implements the Socrates' Reference
    abstract writer class. It outputs all code lines for a variable that is not
    an argument.
    """


    def __init__(
        self
        ,block
        ,depth
        ,parent
    ):
        """
        Initializes this new variable writer with the given block and parent
        writer.

        Parameters
        ----------
        block : socref_glsl.Block.VariableBlock
                The variable block this writer uses to generate its output.
        depth : int
                The indentation depth of this writer's output.
        parent : socref.Abstract.AbstractWriter
                 The parent writer of this writer. This must be a shader or
                 structure writer.
        """
        super().__init__(parent)
        self.__block = block
        self.__depth = depth


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        if not self.__block.inStructure():
            ret.addBlank(Settings.H2)
        ret.setDepth(self.__depth)
        ret.add("/*!")
        ret.addText(self.__block._p_description,Settings.COLS," * ")
        ret.add(" */")
        ret.add(self.__block._p_type.replace("@",self.__block._p_name)+";")
        return ret
