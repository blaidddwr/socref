"""
Contains the NamespaceWriter class.
"""
from ..Settings import Settings
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class NamespaceWriter(WriterBase):
    """
    This is the namespace writer class. It implements the Socrates' Reference
    abstract writer class. It outputs all code lines for a namespace
    declaration.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new namespace writer and its children writers with the
        given namespace block and parent head writer. The given namespace block
        is used by this writer to find its corresponding reader, initialize its
        children writers, and generate its output.

        Parameters
        ----------
        block : NamespaceBlock
                The namespace block.
        parent : HeadWriter
                 The parent head writer.
        """
        super().__init__(parent)
        self.__block = block
        self.__reader = None


    def _footer_(
        self
    ):
        ret = Code(Settings.INDENT)
        for scope in self.__block.scope():
            ret.add("}")
        if self.__block.parent():
            ret.add("}")
        return ret


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.addBlank(Settings.H1)
        for scope in self.__block.scope():
            ret.add("namespace "+scope+" {")
        if self.__block.parent():
            ret.add("/**")
            ret.addText(self.__block._p_description,Settings.COLS," * ")
            ret.add(" */")
            ret.add("namespace "+self.__block._p_name+" {")
        return ret


    def _link_(
        self
    ):
        pass
