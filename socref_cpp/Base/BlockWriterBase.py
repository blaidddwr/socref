"""
Contains the BlockWriterBase class.
"""
from ..Settings import Settings
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class BlockWriterBase(WriterBase):
    """
    This is the block writer base class. It implements the Socrates' Reference
    abstract writer class. It outputs all code lines for a block style
    declaration. Block styles include classes, enumerations, and unions. The
    closing bracket line is done with the footer interface which allows for this
    block writer base to accommodate children writers for classes.
    """


    def __init__(
        self
        ,block
        ,depth
        ,name
        ,parent
    ):
        """
        Initializes this new block writer base with the given block, indentation
        depth of this writer's output, name to use for this block declaration,
        and parent writer. The given block must be a class, enumeration, or
        union and is used by this writer to generate its output. The given
        parent writer must be a header or class writer.

        Parameters
        ----------
        block : AbstractBlock
                The block.
        depth : int
                The indentation depth.
        name : string
               The name.
        parent : AbstractWriter
                 The parent writer.
        """
        super().__init__(parent)
        self.__block = block
        self.__depth = depth
        self.__name = name


    def _block_(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : AbstractBlock
                 The class, enumeration, or union block of this writer.
        """
        return self.__block


    def _footer_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.setDepth(self.__depth)
        ret.add("};")
        ret.add(self._postLines_())
        return ret


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.setDepth(self.__depth)
        if self.__block.parent()._TYPE_ == "Namespace" and self.__block._TYPE_ == "Class":
            ret.addBlank(Settings.H1)
        else:
            ret.addBlank(Settings.H2)
        ret.add("/*!")
        ret.addText(self.__block._p_description,Settings.COLS," * ")
        ret.add(" */")
        if self.__block.parent()._TYPE_ == "Class":
            ret.add("public:")
        ret.add(
            self.__name
            + " "
            + self.__block._p_name
            + (":" if self.__block._TYPE_ == "Class" and self.__block.parents() else "")
        )
        ret.setDepth(self.__depth+1)
        ret.add(self._preLines_())
        ret.setDepth(self.__depth)
        ret.add("{")
        ret.setDepth(self.__depth+1)
        ret.add(self._lines_())
        return ret


    def _lines_(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : list
                 Lines of code added inside the block, after the opening bracket
                 line and before the final closing bracket line.
        """
        return []


    def _postLines_(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : list
                 Lines of post code added after the block's closing bracket
                 line.
        """
        return []


    def _preLines_(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : list
                 Lines of code written before the opening bracket of the code
                 block.
        """
        return []
