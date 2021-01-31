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
        block : object
                Detailed description.
        depth : object
                Detailed description.
        parent : object
                 Detailed description.
        """
        super().__init__(parent)
        self.__b = block
        self.__depth = depth
        self.__r = self.lookup(block.key())


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
        ret.add(self.__b.decorators())
        if self.__b.hasArguments():
            ret.add("def "+self.__b._p_name+"(")
            ret.setDepth(self.__depth+1)
            self.__arguments_(ret)
            ret.setDepth(self.__depth)
            ret.add("):")
        else:
            ret.add("def "+self.__b._p_name+"():")
        ret.setDepth(self.__depth+1)
        ret.add('"""')
        ret.addText(self.__b._p_description,80)
        ret.add('"""')
        if self.__r:
            ret.add(self.__r.lines())
        else:
            ret.add("pass")
        return ret


    def __arguments_(
        self
        ,code
    ):
        """
        Detailed description.

        Parameters
        ----------
        code : object
               Detailed description.
        """
        first = True
        for (name,assignment,t,d) in self.__b.arguments():
            l = []
            if first:
                first = False
                l.append(",")
            l.append(name)
            if assignment:
                l += [" = ",assignment]
            ret.add("".join(l))
