"""
Contains the HeadWriter class.
"""
from ..Settings import Settings
from .ClassWriter import ClassWriter
from .NamespaceWriter import NamespaceWriter
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class HeadWriter(WriterBase):
    """
    This is the head writer class. It implements the Socrates' Reference
    abstract writer class. It outputs the header code lines of a header file.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new head writer and its children writers with the given
        block and parent parser. The given block must be a name space or class
        block that this writer uses to find its corresponding reader, initialize
        its children writers, and generate its output.

        Parameters
        ----------
        block : AbstractBlock
                The block.
        parent : Parser
                 The parser.
        """
        super().__init__(parent)
        self.__block = block
        self.__reader = None
        if block._TYPE_ == "Namespace":
            NamespaceWriter(block,self)
        elif block._TYPE_ == "Class":
            ClassWriter(block,0,self)
        else:
            raise RuntimeError("Unknown block type given.")


    def _footer_(
        self
    ):
        ret = Code(Settings.INDENT)
        for scope in self.__block.scope():
            ret.add("}")
        ret.addBlank(Settings.H2)
        ret.add("#endif")
        return ret


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        name = (
            self.__block._p_name.upper() if self.__block._p_name
            else Settings.GLOBAL_NS_KEY.upper()
        )
        define = "_".join((n.upper() for n in self.__block.scope()+[name])) + "_H"
        ret.add(["#ifndef "+define,"#define "+define])
        if self.__reader:
            ret.add(self.__reader.macros())
        for name in self.__block.scope():
            ret.add("namespace "+name+" {")
        if self.__reader:
            ret.add(self.__reader.header())
        return ret


    def _link_(
        self
    ):
        self.__reader = self.lookup(self.__block.key()+Settings.HDR_EXT)
