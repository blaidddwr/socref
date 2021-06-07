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
        parent writer.

        Parameters
        ----------
        block : FunctionBlock
                The function block this writer uses to find its corresponding
                reader and generate its output.
        parent : ShaderWriter
                 The parent shader writer of this writer.
        """
        super().__init__(parent)
        self.__block = block
        self.__reader = self.lookup(block.key())
        self.__code = None


    def _header_(
        self
    ):
        self.__code = Code(Settings.INDENT)
        self.__code.addBlank(Settings.H1)
        self.__addComments_()
        self.__addHeader_()
        self.__code.add("{")
        if self.__reader:
            self.__code.setDepth(1)
            self.__code.add(self.__reader.lines())
            self.__code.setDepth(0)
        self.__code.add("}")
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
        for (name,type_,description) in self.__block.arguments():
            if first:
                self.__code.add(type_+" "+name)
                first = False
            else:
                self.__code.add(","+type_+" "+name)


    def __addComArguments_(
        self
    ):
        """
        Adds function argument comment lines to this writer's code.
        """
        arguments = self.__block.arguments()
        if arguments:
            self.__code.add(" *")
            for (name,type_,description) in arguments:
                text = "@param "+name+" "
                after = " "*len(text)
                text += description
                self.__code.addText(text,Settings.COLS," * ",after)


    def __addComReturn_(
        self
    ):
        """
        Adds function return comment lines to this writer's code.
        """
        if self.__block._p_returnDescription:
            self.__code.add(" *")
            text = "@return "
            after = " "*len(text)
            text += self.__block._p_returnDescription
            self.__code.addText(text,Settings.COLS," * ",after)


    def __addComments_(
        self
    ):
        """
        Adds function comment lines to this writer's code.
        """
        self.__code.add("/*!")
        self.__code.addText(self.__block._p_description,Settings.COLS," * ")
        self.__addComArguments_()
        self.__addComReturn_()
        self.__code.add(" */")


    def __addHeader_(
        self
    ):
        """
        Adds function header lines of code to this writer's code.
        """
        if self.__block:
            self.__code.add(self.__block._p_returnType+" "+self.__block._p_name+"(")
            self.__code.setDepth(1)
            self.__addArguments_()
            self.__code.setDepth(0)
            self.__code.add(")")
        else:
            self.__code.add(self.__block._p_returnType+" "+self.__block._p_name+"()")
