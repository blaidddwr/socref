"""
Contains the FunctionReader class.
"""
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
        name : object
               Detailed description.
        end : object
              Detailed description.
        parent : object
                 Detailed description.
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
        self.__skipDocString_()
        self.__skipBlank_()
        (i,line) = self.read()
        ind = i
        self.__lines.append(line)
        while True:
            self.save()
            (i,line) = self.read()
            if line is None:
                break
            if not line:
                self.__lines.append("")
            else:
                diff = i-ind
                if diff >= 0:
                    self.__lines.append(" "*diff + line)
                else:
                    self.restore()
                    break
            self.discard()
        while self.__lines[-1] == "":
            self.__lines.pop()


    def __skipBlank_(
        self
    ):
        """
        Detailed description.
        """
        while True:
            self.save()
            (i,line) = self.read()
            if line is None or line:
                self.restore()
                return
            self.__lines.append("")
            self.discard()


    def __skipDocString_(
        self
    ):
        """
        Detailed description.
        """
        count = 0
        while count < 2:
            self.save()
            (i,line) = self.read()
            if line is None:
                return
            if line == '"""':
                count += 1
            elif line and count == 0:
                self.restore()
                return
            self.discard()


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
