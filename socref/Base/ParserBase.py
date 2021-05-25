"""
Contains the ParserBase class.
"""
from ..Abstract.AbstractParser import *
from ..Abstract.AbstractReader import *
from ..Abstract.AbstractWriter import *
from ..Error.ScanError import *
from os import makedirs
from os.path import dirname
from os.path import exists as pathExists
from os.path import isfile
from os.path import join as pathJoin




class ParserBase(AbstractParser):
    """
    This is the parser base class. It partially implements the abstract parser
    class. This base class implements all interfaces for accessing the current
    file being read, the call operator for executing parsing itself, reader
    lookup, and setting the root path.
    """


    def __init__(
        self
    ):
        super().__init__()
        self.__rootPath = ""
        self.__readers = {}
        self.__paths = []
        self.__update = None
        self.__io = None
        self.__stack = []


    def __call__(
        self
        ,update
    ):
        assert(self.__rootPath)
        ret = {}
        try:
            self.__update = update
            self.__paths = self._pathList_()
            filePaths = [p[0] for p in self.__paths]
            if len(set(filePaths)) != len(filePaths):
                raise ScanError("Duplicate file names generated for parsing.")
            self.__readAll_()
            self.__writeAll_()
            ret = self.__unknown_()
        finally:
            self.__readers = {}
            self.__paths = []
            self.__update = None
        return ret


    def addLookup(
        self
        ,key
        ,reader
    ):
        assert(isinstance(reader,AbstractReader))
        if key in self.__readers:
            raise ScanError("An abstract reader set a duplicate key.")
        self.__readers[key] = reader


    def discard(
        self
    ):
        if self.__io is None:
            raise ScanError("Parser cannot discard without open file.")
        if not self.__stack:
            raise ScanError("Parser cannot discard when position stack is empty.")
        self.__stack.pop()


    def lookup(
        self
        ,key
    ):
        return self.__readers.get(key,None)


    def read(
        self
    ):
        if self.__io is None:
            raise ScanError("Parser cannot read without open file.")
        line = self.__io.readline()
        if not line:
            return (None,None)
        indent = len(line)-len(line.lstrip(' '))
        return (indent,line.strip())


    def restore(
        self
    ):
        if self.__io is None:
            raise ScanError("Parser cannot restore without open file.")
        if not self.__stack:
            raise ScanError("Parser cannot restore when position stack is empty.")
        self.__io.seek(self.__stack.pop())


    def save(
        self
    ):
        if self.__io is None:
            raise ScanError("Parser cannot save without open file.")
        self.__stack.append(self.__io.tell())


    def setRootPath(
        self
        ,path
    ):
        assert(not self.__rootPath)
        self.__rootPath = path


    def __readAll_(
        self
    ):
        """
        Reads all source code files from this parser's path list, saving all
        generated readers to this parser's reader lookup table. If None is
        returned by the reader interface for a given path then it is ignored and
        nothing is added to the lookup table.
        """
        count = 0
        for (path,block) in self.__paths:
            try:
                reader = self._reader_(block)
                if reader is not None:
                    if not isinstance(reader,AbstractReader):
                        raise ScanError("Returned object is not an abstract reader.")
                    rp = pathJoin(self.__rootPath,path)
                    if isfile(rp):
                        self.__io = open(rp,"r")
                        reader()
            finally:
                self.__io = None
                self.__stack = []
            count += 1
            self.__update(count*50/len(self.__paths))


    def __unknown_(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : dictionary
                 All read in reader code lines that were not used when writing
                 source code back out to files, where the key is the reader key
                 and the value is the unused lines.
        """
        ret = {}
        for key in self.__readers:
            u = self.__readers[key].unknown()
            if u:
                ret[key] = u
        return ret


    def __writeAll_(
        self
    ):
        """
        Writes all source code files from this parser's path list.
        """
        count = 0
        for (path,block) in self.__paths:
            rp = pathJoin(self.__rootPath,path)
            if not pathExists(dirname(rp)):
                makedirs(dirname(rp))
            writer = self._writer_(block)
            if not isinstance(writer,AbstractWriter):
                raise ScanError("Returned object is not an abstract writer.")
            new = "\n".join(writer()) + "\n"
            if pathExists(rp):
                old = open(rp,"r").read()
                if old != new:
                    open(rp,"w").write(new)
            else:
                open(rp,"w").write(new)
            count += 1
            self.__update(50 + count*50/len(self.__paths))
