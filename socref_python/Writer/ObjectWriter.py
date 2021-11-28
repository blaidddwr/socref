"""
Contains the ObjectWriter class.
"""
from ..Settings import Settings
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




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
        Initializes this new object writer with the given object block used to
        generate this writer's output, indentation depth of this writer's
        output, and parent class writer.

        Parameters
        ----------
        block : ObjectBlock
                The object block.
        depth : int
                The indentation depth.
        parent : ClassWriter
                 The parent class writer.
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
