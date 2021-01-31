"""
Contains the FunctionReader class.
"""
from . import skipDocString
from socref.Abstract.AbstractReader import *




class FunctionReader(AbstractReader):
    """
    Detailed description.
    """


    def __init__(
        self
        ,name
        ,end
        ,parent
    ):
        """
        asdf

        Parameters
        ----------
        name : 
        end : 
        parent : 
        """
        super().__init__(parent)
        self._setKey_(parent.key()+"."+name)
        self.__end = end
        self.__lines = []


    def lines(
        self
    ):
        """
        Detailed description.
        """
        ret = self.__lines
        self.__lines = []
        return ret


    def unknown(
        self
    ):
        """
        Detailed description.
        """
        return self.__lines


    def _scan_(
        self
    ):
        """
        Detailed description.
        """
        self.__skipEnd_()
        skipDocString(self)
        self.__scanLines_()


    def __scanLines_(
        self
    ):
        """
        Detailed description.
        """
        ind = None
        while True:
            self.save()
            (i,line) = self.read()
            if line is None:
                break
            if not line:
                self.__lines.append("")
            else:
                if ind is None:
                    ind = i
                diff = i-ind
                if diff >= 0:
                    self.__lines.append(" "*diff + line)
                else:
                    self.restore()
                    break
            self.discard()
        while self.__lines[-1] == "":
            self.__lines.pop()


    def __skipEnd_(
        self
    ):
        """
        Detailed description.
        """
        if not self.__end.endswith("):"):
            while True:
                (i,line) = self.read()
                if not line or line.endswith("):"):
                    break
