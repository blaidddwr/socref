"""
Contains the EnumerationWriter class.
"""
from ..Settings import Settings
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class EnumerationWriter(WriterBase):
    """
    This is the enumeration writer class. It implements the Socrates' Reference
    abstract writer class. It outputs all code lines for an enumeration
    declaration.
    """


    def __init__(
        self
        ,block
        ,depth
        ,parent
    ):
        """
        Initializes this new enumeration writer with the given enumeration
        block, indentation depth of this writer's output, and parent writer. The
        given enumeration block is used by this writer to find its corresponding
        reader and generate its output. The given parent writer must be a name
        space or class writer.

        Parameters
        ----------
        block : EnumerationBlock
                The enumeration block.
        depth : int
                The indentation depth.
        parent : AbstractWriter
                 The parent writer.
        """
        super().__init__(parent)
        self.__block = block
        self.__depth = depth
        self.__reader = None


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.setDepth(self.__depth)
        ret.addBlank(Settings.H2)
        ret.add("/**")
        ret.addText(self.__block._p_description,Settings.COLS," * ")
        ret.add(" */")
        ret.add(
            ("public: " if self.__block.parent()._TYPE_ == "Class" else "")
            + "enum class "
            + self.__block._p_name
        )
        ret.add("{")
        if self.__reader:
            ret.setDepth(self.__depth+1)
            ret.add(self.__reader.lines())
        ret.setDepth(self.__depth)
        ret.add("};")
        return ret


    def _link_(
        self
    ):
        pass
