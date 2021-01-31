"""
Contains the FunctionWriter class.
"""
from socref.Abstract.AbstractWriter import *
from socref.Output.Code import *




class FunctionWriter(AbstractWriter):
    """
    Detailed description.
    """


    def __init__(
        self
        ,block
        ,depth
        ,parent
    ):
        """
        asdf

        Parameters
        ----------
        block : 
        depth : 
        parent : 
        """
        super().__init__(parent)
        self.__block = block
        self.__depth = depth
        self.__reader = self.lookup(block.key())


    def _footer_(
        self
    ):
        """
        Detailed description.
        """
        return []


    def _header_(
        self
    ):
        """
        Detailed description.
        """
        ret = Code("    ")
        ret.addBlank(2)
        ret.setDepth(self.__depth)
        ret.add(self.__block.decorators())
        arguments = self.__block.arguments()
        if arguments:
            ret.add("def "+self.__block._p_name+"(")
            ret.setDepth(self.__depth+1)
            self.__arguments_(ret,arguments)
            ret.setDepth(self.__depth)
            ret.add("):")
        else:
            ret.add("def "+self.__block._p_name+"():")
        ret.setDepth(self.__depth+1)
        if self.__block._p_description:
            ret.add('"""')
            ret.addText(self.__block._p_description,80)
            arguments = self.__block.arguments(False)
            if arguments:
                ret.addBlank(1)
                self.__docArguments_(ret,arguments)
            returns = self.__block.returns()
            if returns:
                ret.addBlank(1)
                self.__returns_(ret,returns)
            ret.add('"""')
        if self.__reader:
            ret.add(self.__reader.lines())
        else:
            ret.add("pass")
        return ret


    @staticmethod
    def __arguments_(
        code
        ,arguments
    ):
        """
        Detailed description.

        Parameters
        ----------
        code : 
        arguments : 
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
    def __docArguments_(
        code
        ,arguments
    ):
        """
        Detailed description.

        Parameters
        ----------
        code : 
        arguments : 
        """
        code.add(["Parameters","----------"])
        for (name,a,type_,text) in arguments:
            h1 = name+" : "
            l = len(h1)
            code.add(h1+type_)
            code.addText(text,80," "*l)


    @staticmethod
    def __returns_(
        code
        ,returns
    ):
        """
        Detailed description.

        Parameters
        ----------
        code : 
        returns : 
        """
        code.add(["Returns","-------"])
        for (name,type_,text) in returns:
            h1 = name+" : "
            l = len(h1)
            code.add(h1+type_)
            code.addText(text,80," "*l)
