"""
Contains the ShaderWriter class.
"""
from ..Settings import *
from socref.Abstract.AbstractWriter import *
from socref.Output.Code import *




class ShaderWriter(AbstractWriter):
    """
    This is the shader writer class. It implements the Socrates' Reference
    abstract writer class. It outputs the header comment description of a
    shader. Children writers for variables, structures, and functions output
    their code lines after the header comment description.
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
        block : socref_glsl.Block.ShaderBlock
                The shader block this writer uses to construct its children
                writers and generate its output.
        parent : socref_glsl.Parser
                 The parser currently outputting its project's source code
                 files.
        """
        super().__init__(parent)
        self.__block = block
        #TODO: add children writers


    def _footer_(
        self
    ):
        return []


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.add("/*!")
        ret.addText(self.__block._p_description,Settings.COLS," * ")
        ret.add(" */")
        return ret
