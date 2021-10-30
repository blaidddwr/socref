"""
Contains the ModuleWriter class.
"""
from ..Settings import Settings
from .ClassWriter import ClassWriter
from .FunctionWriter import FunctionWriter
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class ModuleWriter(WriterBase):
    """
    This is the module writer class. It implements the Socrates' Reference
    abstract writer class. It outputs the header and custom footer code lines
    for a module. The header consists of any initial special comment lines, the
    doc string, and then import lines. Children writers for functions and
    classes output their code lines between the header and footer.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new module writer and its children writers with the
        given block and parent parser. The given block must be a package,
        module, or class that this writer uses to find its corresponding reader,
        initialize its children writers, and generate its output. If the given
        block is a class then this writer creates a child class writer with the
        given class block.

        Parameters
        ----------
        block : AbstractBlock
                The block.
        parent : Parser
                 The parser.
        """
        super().__init__(parent)
        self.__block = block
        self.__reader = self.lookup(block.key(True))
        if block._TYPE_ == "Class":
            ClassWriter(block,0,self)
        else:
            for child in block:
                if child._TYPE_ == "Function":
                    FunctionWriter(child,0,self)
                elif child._TYPE_ == "Class" and child.isInfile():
                    ClassWriter(child,0,self)


    def _footer_(
        self
    ):
        ret = Code(Settings.INDENT)
        if self.__reader:
            f = self.__reader.footer()
            if f:
                ret.addBlank(Settings.H2)
                ret.add(f)
        return ret


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        if self.__reader:
            ret.add(self.__reader.preHeader())
        ret.add('"""')
        if self.__block._TYPE_ == "Class":
            ret.addText("Contains the "+self.__block._p_name+" class.",Settings.COLS)
        else:
            ret.addText(self.__block._p_description,Settings.COLS)
        ret.add('"""')
        if self.__reader:
            ret.add(self.__reader.header())
        return ret
