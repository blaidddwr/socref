"""
Contains the VariableWriter class.
"""
from ..Settings import Settings
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




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
        Initializes this new variable writer with the given variable block,
        indentation depth of this writer's output, and parent writer. The given
        variable block is used by this writer to generate its output. The given
        parent writer must be a shader or structure writer.

        Parameters
        ----------
        block : VariableBlock
                The variable block.
        depth : int
                The indentation depth.
        parent : AbstractWriter
                 The parent writer.
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
