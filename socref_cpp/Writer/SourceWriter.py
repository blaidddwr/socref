"""
Contains the SourceWriter class.
"""
from ..Settings import Settings
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class SourceWriter(WriterBase):
    """
    This is the source writer class. It implements the Socrates' Reference
    abstract writer class. It outputs the header code lines of a source file.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new source writer and its children writers with the
        given block and parent parser. The given block must be a namespace or
        class block that this writer uses to find its corresponding reader,
        initialize its children writers, and generate its output.

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


    def _footer_(
        self
    ):
        ret = Code(Settings.INDENT)
        for scope in self.__block.scope():
            ret.add("}")
        if self.__block._TYPE_ == "Namespace":
            ret.add("}")
        return ret


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.add(
            '#include "__init__.h"' if self.__block._TYPE_ == "Namespace"
            else '#include "'+self.__block._p_name+'.h"'
        )
        if self.__reader:
            ret.add(self.__reader.header())
        scope = self.__block.scope()
        if self.__block._TYPE_ == "Namespace":
            scope.append(self.__block._p_name)
        if scope:
            ret.addBlank(Settings.H2)
            for name in scope:
                ret.add("namespace "+name+" {")
        return ret


    def _link_(
        self
    ):
        self.__reader = self.lookup(self.__block.key(True)+".cpp")
