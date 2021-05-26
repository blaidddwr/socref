"""
Contains the ObjectWriter class.
"""
from ..Settings import *
from socref.Base.WriterBase import *
from socref.Output.Code import *




class ObjectWriter(WriterBase):
    """
    This is the object writer class. It implements the Socrates' Reference
    abstract writer class. It outputs all code lines for an object that is not
    an argument.
    """


    def __init__(
        self
        ,block
        ,depth
        ,parent
    ):
        """
        Initializes this new object writer with the given object block, indent
        depth, and parent writer.

        Parameters
        ----------
        block : socref_python.Block.ObjectBlock
                The object block this writer uses to generate its output.
        depth : int
                The indentation depth of this writer's output.
        parent : socref_python.Writer.ClassWriter
                 The parent class writer of this writer.
        """
        super().__init__(parent)
        self.__block = block
        self.__depth = depth


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.addBlank(Settings.H2)
        ret.setDepth(self.__depth)
        ret.add("#")
        ret.addText(self.__block._p_description,Settings.COLS,"# ")
        ret.add("#")
        ret.add(self.__block._p_name+" = "+self.__block._p_assignment)
        return ret
