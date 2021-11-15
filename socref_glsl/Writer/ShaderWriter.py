"""
Contains the ShaderWriter class.
"""
from ..Settings import Settings
from .FunctionWriter import FunctionWriter
from .StructureWriter import StructureWriter
from .VariableWriter import VariableWriter
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class ShaderWriter(WriterBase):
    """
    This is the shader writer class. It implements the Socrates' Reference
    abstract writer class. It outputs the header comment description of a
    shader. It outputs the necessary special main function in the footer.
    Children writers for variables, structures, and functions output their code
    lines after the header comment description.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new shader writer with the given shader block and
        parent parser. The given shader block is used by this writer to find the
        special main function reader, initialize its children writers, and
        generate its output.

        Parameters
        ----------
        block : ShaderBlock
                The shader block.
        parent : Parser
                 The parent parser.
        """
        super().__init__(parent)
        self.__block = block
        self.__reader = None
        for child in block:
            if child._TYPE_ == "Variable":
                VariableWriter(child,0,self)
            elif child._TYPE_ == "Structure":
                StructureWriter(child,self)
            elif child._TYPE_ == "Function":
                FunctionWriter(child,self)


    def _footer_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.addBlank(Settings.H1)
        ret.add("/*!")
        ret.add(" * Special entry point function.")
        ret.add(" */")
        ret.add("void main()")
        ret.add("{")
        ret.setDepth(1)
        if self.__reader:
            ret.add(self.__reader.lines())
        ret.setDepth(0)
        ret.add("}")
        return ret


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.add("#version "+self.__block._p_version)
        ret.add("/*!")
        ret.addText(self.__block._p_description,Settings.COLS," * ")
        ret.add(" */")
        return ret


    def _link_(
        self
    ):
        self.__reader = self.lookup(self.__block.key()+".main()")
