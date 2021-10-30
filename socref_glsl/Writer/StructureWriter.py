"""
Contains the StructureWriter class.
"""
from ..Settings import Settings
from .VariableWriter import VariableWriter
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class StructureWriter(WriterBase):
    """
    This is the structure writer class. It implements the Socrates' Reference
    abstract writer class. It outputs the header and footer code lines for a
    structure. Children writers for nested variables output their code after the
    header.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new structure writer with the given structure block and
        parent shader writer. The given structure block is used by this writer
        to initialize its children writers and generate its output.

        Parameters
        ----------
        block : StructureBlock
                The structure block.
        parent : ShaderWriter
                 The parent shader writer.
        """
        super().__init__(parent)
        self.__block = block
        for child in block:
            if child._TYPE_ == "Variable":
                VariableWriter(child,1,self)


    def _footer_(
        self
    ):
        ret = Code(Settings.INDENT)
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
        ret.add("/*!")
        ret.addText(self.__block._p_description,Settings.COLS," * ")
        ret.add(" */")
        ret.add(self.__block._p_type)
        ret.add("{")
        return ret
