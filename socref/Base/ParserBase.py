"""
Contains the ParserBase class.
"""
from ..Abstract.AbstractParser import AbstractParser
from ..Abstract.AbstractReader import AbstractReader
from ..Abstract.AbstractWriter import AbstractWriter
from ..Error.ScanError import ScanError
from os import makedirs
from os.path import (
    dirname
    ,exists as pathExists
    ,isfile
    ,join as pathJoin
)




class ParserBase(AbstractParser):
    """
    This is the parser base class. It partially implements the abstract parser
    class. This base class implements all interfaces for accessing the current
    file being read, the call operator for executing parsing itself, reader
    lookup, and setting the root path.

    An interface for generating a path list with its associated optional reader
    and required writer are provided for convenience. This path list is used by
    this base class in its implemented call operator for parsing and generating
    source code files.
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


    def _pathList_(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : list
                 A list of tuples. Each tuple contains a relative path to a
                 source code file that is parsed, an optional abstract reader
                 that parses the source code file, and required abstract writer
                 that writes the output of the source code file, in that order.
                 Each path is relative to the root path of the project being
                 parsed. If no abstract reader is required then none is used as
                 a placeholder for that tuple.
        """
        return ()


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
        for (path,reader,writer) in self.__paths:
            try:
                if reader is not None:
                    if not isinstance(reader,AbstractReader):
                        raise ScanError("2nd object of path list tuple is not an abstract reader.")
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
        for (path,reader,writer) in self.__paths:
            rp = pathJoin(self.__rootPath,path)
            if not pathExists(dirname(rp)):
                makedirs(dirname(rp))
            if not isinstance(writer,AbstractWriter):
                raise ScanError("3rd object of path list tuple is not an abstract writer.")
            new = "\n".join(writer()) + "\n"
            if pathExists(rp):
                old = open(rp,"r").read()
                if old != new:
                    open(rp,"w").write(new)
            else:
                open(rp,"w").write(new)
            count += 1
            self.__update(50 + count*50/len(self.__paths))
