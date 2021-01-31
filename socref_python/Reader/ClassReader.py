"""
Contains the ClassReader class.
"""
from . import skipDocString
from .FunctionReader import *
from re import compile as reCompile
from socref.Abstract.AbstractReader import *




class ClassReader(AbstractReader):
    """
    Detailed description.
    """
    __functionRE = reCompile('^def +([a-zA-Z_]+\w*)\((.*)')
    __classRE = reCompile('^class +([a-zA-Z_]+\w*)\([\w\._,\s]*\):')


    def __init__(
        self
        ,name
        ,parent
    ):
        """
        asdf

        Parameters
        ----------
        name : 
        parent : 
        """
        super().__init__(parent)
        self._setKey_(parent.key()+"."+name)
        self.__lines = []
        self.__ind = None


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
        skipDocString(self)
        self.__scanLines_()
        self.__scanDefs_()


    def __scanDefs_(
        self
    ):
        """
        Detailed description.
        """
        while True:
            self.save()
            (i,line) = self.read()
            if line is None:
                return
            if line:
                if self.__ind is None:
                    self.__ind = i
                if i < self.__ind:
                    self.restore()
                    return
                match = self.__functionRE.match(line)
                if match:
                    name = match.group(1)
                    end = match.group(2)
                    reader = FunctionReader(name,end,self)
                    reader()
                    continue
                match = self.__classRE.match(line)
                if match:
                    name = match.group(1)
                    reader = ClassReader(name,self)
                    reader()
                    continue
            self.discard()


    def __scanLines_(
        self
    ):
        """
        Detailed description.
        """
        while True:
            self.save()
            (i,line) = self.read()
            if not line:
                return
            else:
                if self.__ind is None:
                    self.__ind = i
                diff = i-self.__ind
                if diff >= 0:
                    self.__lines.append(" "*diff + line)
                else:
                    self.restore()
                    return
            self.discard()
