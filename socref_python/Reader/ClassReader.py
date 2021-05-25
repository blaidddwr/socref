"""
Contains the ClassReader class.
"""
from . import skipDocString
from .FunctionReader import *
from re import compile as reCompile
from socref.Base.ReaderBase import *




class ClassReader(ReaderBase):
    """
    This is the class reader class. It implements the Socrates' Reference
    abstract reader class. It parses a class; saving custom header lines of
    code, all methods, and all nested classes of the parsed class. Parsed
    function and class definitions are added as new reader children.
    """
    __functionRE = reCompile('^def +([a-zA-Z_]+\w*)\((.*)')
    __classRE = reCompile('^class +([a-zA-Z_]+\w*)\([\w\._,\s]*\):')


    def __init__(
        self
        ,name
        ,parent
    ):
        """
        Initializes this new class reader with the given class name and parent
        reader.

        Parameters
        ----------
        name : string
               The name of the class this reader will parse.
        parent : socref.Abstract.AbstractReader
                 The parent reader that discovered the header code line of the
                 class that this reader will parse.
        """
        super().__init__(parent)
        self._setKey_(parent.key()+"."+name)
        self.__lines = []
        self.__ind = None


    def lines(
        self
    ):
        """
        Getter method. This can only be called once, after which an empty list
        is returned.

        Returns
        -------
        result : list
                 Lines of custom header code parsed by this class reader.
        """
        ret = self.__lines
        self.__lines = []
        return ret


    def unknown(
        self
    ):
        return self.__lines


    def _scan_(
        self
    ):
        skipDocString(self)
        self.__scanLines_()
        self.__scanDefs_()


    def __scanDefs_(
        self
    ):
        """
        Scans for any function or class definitions within this reader's parsed
        class. New function or class readers are created as children of this
        reader for each definition found.
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
                    reader = FunctionReader(name,end.endswith("):"),self)
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
        Scans for any custom header lines of code at the beginning of this
        reader's parsed class, positioned right after the header and doc string.
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
