"""
Contains the ModuleWriter class.
"""
from ..Settings import *
from .ClassWriter import *
from .FunctionWriter import *
from socref.Abstract.AbstractWriter import *
from socref.Output.Code import *




class ModuleWriter(AbstractWriter):
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
        given block and parent parser.

        Parameters
        ----------
        block : socref.Abstract.AbstractBlock
                A package, module, or class block that this writer uses to find
                its corresponding reader, initialize its children writers, and
                generate output. If the given block is a class then the reader
                find is the dummy module container and a new class writer child
                is created with the same class block.
        parent : socref_python.Parser
                 The parser currently outputting its project's source code
                 files.
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
