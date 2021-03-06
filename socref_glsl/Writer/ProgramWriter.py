"""
Contains the ProgramWriter class.
"""
from ..Settings import Settings
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class ProgramWriter(WriterBase):
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
        Initializes this new program writer with the given program block and
        parent parser. The given program block is used by this writer to
        generate its output.

        Parameters
        ----------
        block : ProgramBlock
                The program block.
        parent : Parser
                 The parent parser.
        """
        super().__init__(parent)
        self.__block = block


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.addText(self.__block._p_description,Settings.COLS)
        return ret
