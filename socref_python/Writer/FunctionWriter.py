"""
Contains the FunctionWriter class.
"""
from ..Settings import *
from socref.Base.WriterBase import *
from socref.Output.Code import *




class FunctionWriter(WriterBase):
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
        self.__code = None


    def _footer_(
        self
    ):
        return []


    def _header_(
        self
    ):
        self.__code = Code(Settings.INDENT)
        self.__code.addBlank(Settings.H2)
        self.__code.setDepth(self.__depth)
        self.__code.add(self.__block.decorators())
        self.__addHeader_()
        self.__code.setDepth(self.__depth+1)
        self.__addDocString_()
        if self.__reader:
            self.__code.add(self.__reader.lines())
        else:
            self.__code.add("pass")
        ret = self.__code
        self.__code = None
        return ret


    def __addArguments_(
        self
    ):
        """
        Adds function argument lines of code to this writer's code.
        """
        first = True
        for (name,assignment,t,d) in self.__block.arguments():
            l = []
            if first:
                first = False
            else:
                l.append(",")
            l.append(name)
            if assignment:
                l += ["=",assignment]
            self.__code.add("".join(l))


    def __addDocArguments_(
        self
    ):
        """
        Adds function argument doc string lines to this writer's code.
        """
        arguments = self.__block.arguments(False)
        if arguments:
            self.__code.addBlank(1)
            self.__code.add(["Parameters","----------"])
            for (name,a,type_,description) in arguments:
                h1 = name+" : "
                l = len(h1)
                self.__code.add(h1+type_)
                self.__code.addText(description,Settings.COLS," "*l)


    def __addDocReturns_(
        self
    ):
        """
        Adds function returns doc string lines to this writer's code.
        """
        returns = self.__block.returns()
        if returns:
            self.__code.addBlank(1)
            self.__code.add(["Returns","-------"])
            for (name,type_,description) in returns:
                h1 = name+" : "
                l = len(h1)
                self.__code.add(h1+type_)
                self.__code.addText(description,Settings.COLS," "*l)


    def __addDocString_(
        self
    ):
        """
        Adds function doc string lines to this writer's code.
        """
        if self.__block._p_description:
            self.__code.add('"""')
            self.__code.addText(self.__block._p_description,Settings.COLS)
            self.__addDocArguments_()
            self.__addDocReturns_()
            self.__code.add('"""')


    def __addHeader_(
        self
    ):
        """
        Adds function header lines of code to this writer's code.
        """
        if self.__block.hasArguments():
            self.__code.add("def "+self.__block._p_name+"(")
            self.__code.setDepth(self.__depth+1)
            self.__addArguments_()
            self.__code.setDepth(self.__depth)
            self.__code.add("):")
        else:
            self.__code.add("def "+self.__block._p_name+"():")
