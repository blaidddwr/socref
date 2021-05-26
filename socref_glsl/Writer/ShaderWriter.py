"""
Contains the ShaderWriter class.
"""
from ..Settings import *
from .FunctionWriter import *
from .StructureWriter import *
from .VariableWriter import *
from socref.Base.WriterBase import *
from socref.Output.Code import *




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
        Initializes this new shader writer and constructs its children writers
        with the given block and parent parser.

        Parameters
        ----------
        block : ShaderBlock
                The shader block this writer uses to find the special main
                function reader, initialize its children writers, and generate
                its output.
        parent : Parser
                 The parser currently outputting its project's source code
                 files.
        """
        super().__init__(parent)
        self.__block = block
        self.__reader = self.lookup(block.key()+".main()")
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
        ret.add("/*!")
        ret.addText(self.__block._p_description,Settings.COLS," * ")
        ret.add(" */")
        return ret
