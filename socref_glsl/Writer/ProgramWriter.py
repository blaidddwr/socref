"""
Contains the ProgramWriter class.
"""
from ..Settings import *
from socref.Abstract.AbstractWriter import *
from socref.Output.Code import *




class ProgramWriter(AbstractWriter):
    """
    This is the program writer class. It implements the Socrates' Reference
    abstract writer class. It outputs the README that is contained within each
    program folder.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new program writer with the given block and parent
        parser.

        Parameters
        ----------
        block : socref_glsl.Block.ProgramBlock
                The program block this writer uses to generate its output.
        parent : socref_glsl.Parser
                 The parser currently outputting its project's source code
                 files.
        """
        super().__init__(parent)
        self.__block = block


    def _footer_(
        self
    ):
        return []


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.addBlank(Settings.H2)
        ret.addText(self.__block._p_description,Settings.COLS)
        return ret
