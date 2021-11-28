"""
Contains the FunctionWriter class.
"""
from ..Settings import Settings
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class FunctionWriter(WriterBase):
    """
    This is the function writer class. It implements the Socrates' Reference
    abstract writer class. It outputs all code lines for a function.
    """


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new function writer with the given function block and
        parent shader writer. The given function block is used by this writer to
        find its corresponding reader and generate its output.

        Parameters
        ----------
        block : FunctionBlock
                The function block.
        parent : ShaderWriter
                 The parent shader writer.
        """
        super().__init__(parent)
        self.__block = block
        self.__reader = None


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.addBlank(Settings.H1)
        ret.add("/*!")
        ret.addText(self.__block._p_description,Settings.COLS," * ")
        arguments = self.__block.arguments()
        if arguments:
            for (name,t,description) in arguments:
                if description:
                    ret.add(" *")
                    ret.add(" * @param "+name)
                    ret.addText(description,Settings.COLS," *        ")
        if self.__block._p_returnDescription:
            ret.add(" *")
            ret.add(" * @return")
            ret.addText(self.__block._p_returnDescription,Settings.COLS," * ")
        ret.add(" */")
        ret.add(self.__block._p_returnType+" "+self.__block._p_name+"(")
        if arguments:
            ret.setDepth(1)
            first = True
            for (name,type_,d) in arguments:
                if first:
                    ret.add(type_+" "+name)
                    first = False
                else:
                    ret.add(","+type_+" "+name)
            ret.setDepth(0)
        ret.add(")")
        ret.add("{")
        if self.__reader:
            ret.setDepth(1)
            ret.add(self.__reader.lines())
            ret.setDepth(0)
        ret.add("}")
        return ret


    def _link_(
        self
    ):
        self.__reader = self.lookup(self.__block.key())
