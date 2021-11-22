"""
Contains the FunctionWriter class.
"""
from ..Block.FunctionBlock import FunctionBlock
from ..Settings import Settings
from socref.Base.WriterBase import WriterBase
from socref.Output.Code import Code




class FunctionWriter(WriterBase):
    """
    This is the function writer class. It implements the Socrates' Reference
    abstract writer class. It outputs all code lines for a function declaration
    or definition.
    """


    def __init__(
        self
        ,block
        ,depth
        ,parent
        ,declaration=True
    ):
        """
        Initializes this new function writer with the given function block,
        indentation depth of this writer's output, parent writer, and
        declaration flag. The given function block is used by this writer to
        find its corresponding reader and generate its output. The given parent
        writer must be a source, name space, or class writer. The given
        declaration flag is true to have this writer output a declaration or
        false to have it output a definition.

        Parameters
        ----------
        block : FunctionBlock
                The function block.
        depth : int
                The indentation depth.
        parent : AbstractWriter
                 The parent writer.
        declaration : bool
                      The declaration flag.
        """
        super().__init__(parent)
        self.__decl = declaration
        self.__block = block
        self.__depth = depth
        self.__reader = None


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.setDepth(self.__depth)
        ret.addBlank(Settings.H2)
        arguments = self.__block.arguments()
        if self.__decl and self.__block._p_description:
            ret.add("/**")
            ret.addText(self.__block._p_description,Settings.COLS," * ")
            for argument in arguments:
                if argument[3]:
                    ret.add(" *")
                    ret.add(" * @param "+argument[0])
                    ret.addText(argument[3],Settings.COLS," *        ")
            if self.__block.hasReturn() and self.__block._p_returnDescription:
                ret.add(" *")
                ret.add(" * @return")
                ret.addText(self.__block._p_returnDescription,Settings.COLS," * ")
            ret.add(" */")
        (before,after) = self.__block.flags(FlagOutput.Code)
        ret.add(
            " ".join(
                self.__block.codeAccess(self.__decl)
                + before
                + self.__block.codeReturn()
                + self.__block.codeName(self.__decl)
            )
            + "("
        )
        if arguments:
            ret.setDepth(self.__depth+1)
            comma = ""
            for argument in arguments:
                ret.add(
                    comma
                    + argument[2]
                    + " "
                    + argument[0]
                    + (" = "+argument[1] if argument[1] else "")
                )
                if not comma:
                    comma = ","
            ret.setDepth(self.__depth)
        preLines = []
        if self.__reader:
            preLines = self.__reader.preLines()
        if self.__decl:
            ret.add(" ".join([")"]+after)+";")
        else:
            ret.add(" ".join([")"]+after)+(":" if preLines else ""))
        if not self.__decl:
            if preLines:
                ret.setDepth(self.__depth+1)
                ret.add(preLines)
                ret.setDepth(self.__depth)
            ret.add("{")
            ret.setDepth(self.__depth+1)
            if self.__reader:
                ret.add(self.__reader.lines())
            ret.setDepth(self.__depth)
            ret.add("}")
        return ret


    def _link_(
        self
    ):
        pass


FlagOutput = FunctionBlock.FlagOutput
