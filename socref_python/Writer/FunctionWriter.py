"""
Contains the FunctionWriter class.
"""
from ..Settings import *
from socref.Abstract.AbstractWriter import *
from socref.Output.Code import *




class FunctionWriter(AbstractWriter):
    """
    This is the function writer class. It implements the Socrates' Reference
    abstract writer class. It outputs all code lines for a function.
    """


    def __init__(
        self
        ,block
        ,depth
        ,parent
    ):
        """
        Initializes this new function writer with the given function block,
        indent depth, and parent writer.

        Parameters
        ----------
        block : socref_python.Block.FunctionBlock
                The function block this writer uses to find its corresponding
                reader and generate its output.
        depth : int
                The indentation depth of this writer's output.
        parent : socref.Abstract.AbstractWriter
                 The parent writer of this writer. This must be a module or
                 class writer.
        """
        super().__init__(parent)
        self.__block = block
        self.__depth = depth
        self.__reader = self.lookup(block.key())


    def _footer_(
        self
    ):
        return []


    def _header_(
        self
    ):
        ret = Code(Settings.INDENT)
        ret.addBlank(Settings.H2)
        ret.setDepth(self.__depth)
        ret.add(self.__block.decorators())
        arguments = self.__block.arguments()
        if arguments:
            ret.add("def "+self.__block._p_name+"(")
            ret.setDepth(self.__depth+1)
            self.__addArguments_(ret,arguments)
            ret.setDepth(self.__depth)
            ret.add("):")
        else:
            ret.add("def "+self.__block._p_name+"():")
        ret.setDepth(self.__depth+1)
        if self.__block._p_description:
            ret.add('"""')
            ret.addText(self.__block._p_description,Settings.COLS)
            arguments = self.__block.arguments(False)
            if arguments:
                ret.addBlank(1)
                self.__addDocArguments_(ret,arguments)
            returns = self.__block.returns()
            if returns:
                ret.addBlank(1)
                self.__addReturns_(ret,returns)
            ret.add('"""')
        if self.__reader:
            ret.add(self.__reader.lines())
        else:
            ret.add("pass")
        return ret


    @staticmethod
    def __addArguments_(
        code
        ,arguments
    ):
        """
        Adds function argument lines of code to the given code output instance
        using the given list of arguments.

        Parameters
        ----------
        code : socref.Output.Code
               Code output instance that this adds argument lines to.
        arguments : list
                    Tuples of argument values used to generate argument lines.
                    Each tuple must contain the name, assignment, type, and
                    description in that order.
        """
        first = True
        for (name,assignment,t,d) in arguments:
            l = []
            if first:
                first = False
            else:
                l.append(",")
            l.append(name)
            if assignment:
                l += ["=",assignment]
            code.add("".join(l))


    @staticmethod
    def __addDocArguments_(
        code
        ,arguments
    ):
        """
        Adds function argument doc string lines to the given code output
        instance using the given list of arguments.

        Parameters
        ----------
        code : socref.Output.Code
               Code output instance that this adds argument doc string lines to.
        arguments : list
                    Tuples of argument values used to generate argument doc
                    string lines. Each tuple must contain the name, assignment,
                    type, and description in that order.
        """
        code.add(["Parameters","----------"])
        for (name,a,type_,text) in arguments:
            h1 = name+" : "
            l = len(h1)
            code.add(h1+type_)
            code.addText(text,Settings.COLS," "*l)


    @staticmethod
    def __addReturns_(
        code
        ,returns
    ):
        """
        Adds function returns doc string lines to the given code output instance
        using the given list of returns.

        Parameters
        ----------
        code : socref.Output.Code
               Code output instance that this adds returns doc string lines to.
        returns : list
                  Tuples of return values used to generate returns doc string
                  lines. Each tuple must contain the name, type, and description
                  in that order.
        """
        code.add(["Returns","-------"])
        for (name,type_,text) in returns:
            h1 = name+" : "
            l = len(h1)
            code.add(h1+type_)
            code.addText(text,Settings.COLS," "*l)
