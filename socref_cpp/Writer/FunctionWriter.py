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
            ret.add("/*!")
            ret.addText(self.__block._p_description,Settings.COLS," * ")
            for (name,a,t,desc) in arguments:
                if desc:
                    ret.add(" *")
                    ret.add(" * @param "+name)
                    ret.addText(desc,Settings.COLS," *        ")
            if self.__block.hasReturn() and self.__block._p_returnDescription:
                ret.add(" *")
                ret.add(" * @return")
                ret.addText(self.__block._p_returnDescription,Settings.COLS," * ")
            ret.add(" */")
        ret.add(self.__block.codeAccess(self.__decl))
        (before,after) = self.__block.flags(
            FlagOutput.HeaderCode if self.__decl else FlagOutput.SourceCode
        )
        ret.add(
            " ".join(
                before
                + self.__block.codeReturn()
                + self.__block.codeName(self.__decl)
            )
            + "("
        )
        if arguments:
            ret.setDepth(self.__depth+1)
            comma = ""
            for (name,assignment,type_,d) in arguments:
                ret.add(
                    comma
                    + type_
                    + " "
                    + name
                    + (" = "+assignment if assignment and self.__decl else "")
                )
                if not comma:
                    comma = ","
            ret.setDepth(self.__depth)
        initLines = []
        if self.__reader:
            initLines = self.__reader.initLines()
        if self.__decl:
            ret.add(" ".join([")"]+after)+";")
        else:
            ret.add(" ".join([")"]+after)+(":" if initLines else ""))
        if not self.__decl:
            if initLines:
                ret.setDepth(self.__depth+1)
                ret.add(initLines)
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
        if not self.__decl:
            self.__reader = self.lookup(self.__block.key())


FlagOutput = FunctionBlock.FlagOutput
