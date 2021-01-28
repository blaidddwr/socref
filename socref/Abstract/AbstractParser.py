"""
Contains the AbstractParser class.
"""
from .AbstractReader import *
from .AbstractWriter import *
from abc import ABC
from abc import abstractmethod
from os import makedirs
from os.path import dirname
from os.path import exists as pathExists
from os.path import isfile
from os.path import join as pathJoin




class AbstractParser(ABC):
    """
    This is the abstract parser class. A parser is the interface implemented by
    a language to parse its source code from a project. The parsing process is
    separated into building a path list, scanning, and building.

    A parser is the interface implemented by a language to parse its source code
    from a project. When the core application parses the source code of a
    project it creates a new abstract parser from the root block of the project
    that is to be parsed. The abstract parser in turn handles parsing all source
    code files using the root block of the project. A root path is also set that
    is used as the root location for all source code directories and/or files.

    The parsing process is separated into building a path list, scanning,
    building, and returning any unknown code fragments. The building a path list
    step builds a list of paths and associated blocks where source file codes
    should exist within the root path. The scanning step reads and scans every
    source code file for each path generated from the previous step. The build
    step generates new contents for each source code file generated from the
    first step, using the associated block from the first step and any scanned
    input from the previous step. The returning any unknown code fragments step
    returns any code fragments that were scanned but never used in the build
    step. If a file does not exist for a given path it is ignored in the scan
    step. Before parsing can begin the root path must be set.

    _DEPRECATED_
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
        """
        Parses the source code of the project of this parser's root block,
        updating its progress with the given callback object. The root path of
        this parser must be set.

        Parameters
        ----------
        update : function
                 Used to update the progress of this scan. It takes one argument
                 that is the progress as a percentage from 1 to 99.
        """
        assert(self.__rootPath):
        ret = {}
        try:
            self.__update = update
            self.__paths = self._pathList_()
            filePaths = [p[0] for p in self.__paths]
            if len(set(filePaths)) != len(filePaths):
                raise ScanError("Duplicate file names generated for parsing.")
            self.__readAll_()
            self.__writeAll_()
            ret = self.__unknowns_()
        finally:
            self.__readers = {}
            self.__paths = []
            self.__update = None
        return ret


    def lookup(
        self
        ,*keys
    ):
        """
        Detailed description.

        Parameters
        ----------
        *keys : object
                Detailed description.
        """
        if not keys:
            return None
        rkey = keys.pop(0)
        reader = self.__readers.get(rkey,None)
        while reader is not None and keys:
            reader = reader[keys.pop(0)]
        return reader


    def peak(
        self
    ):
        """
        Detailed description.
        """
        if self.__io is None:
            raise ScanError("Parser cannot peak without open file.")
        i = self.__io.tell()
        ret = self.__read_()
        self.__io.seek(i)
        return ret


    def read(
        self
    ):
        """
        Detailed description.
        """
        if self.__io is None:
            raise ScanError("Parser cannot read without open file.")
        return self.__read_()


    def restore(
        self
    ):
        """
        Detailed description.
        """
        if self.__io is None:
            raise ScanError("Parser cannot restore without open file.")
        if not self.__stack:
            raise ScanError("Parser cannot restore when position stack is empty.")
        self.__io.seek(self.__stack.pop())


    def save(
        self
    ):
        """
        Detailed description.
        """
        if self.__io is None:
            raise ScanError("Parser cannot save without open file.")
        self.__stack.append(self.__io.tell())


    def setRootPath(
        self
        ,path
    ):
        """
        Sets the root path of this parser. This can only be called once when
        this parser's root path is empty.

        Parameters
        ----------
        path : string
               The root path of this parser.
        """
        assert(not self.__rootPath)
        self.__rootPath = path


    @abstractmethod
    def _pathList_(
        self
    ):
        """
        Detailed description.
        """
        pass


    @abstractmethod
    def _reader_(
        self
        ,path
        ,block
    ):
        """
        Detailed description.

        Parameters
        ----------
        path : object
               Detailed description.
        block : object
                Detailed description.
        """
        pass


    @abstractmethod
    def _writer_(
        self
        ,path
        ,block
    ):
        """
        Detailed description.

        Parameters
        ----------
        path : object
               Detailed description.
        block : object
                Detailed description.
        """
        pass


    def __read_(
        self
    ):
        """
        Detailed description.
        """
        line = self.__io.readline()
        if not line:
            return None
        indent = len(line)-len(line.lstrip(' '))
        return (indent,line.strip())


    def __readAll_(
        self
    ):
        """
        Detailed description.
        """
        count = 0
        for (path,block) in self.__paths:
            if isfile(path):
                self.__io = open(pathJoin(self.__rootPath,path),"r")
                try:
                    self.__ifile = ifile
                    reader = self._reader_(path,block)
                    if not isinstance(reader,AbstractReader):
                        raise ScanError("Returned object is not an abstract reader.")
                    reader()
                    if reader.key() in self.__readers:
                        raise ScanError("An abstract reader set a duplicate key.")
                    self.__readers[reader.key()] = reader
                finally:
                    self.__io = None
                    self.__stack = []
            count += 1
            self.__update(count*50/len(self.__paths))


    def __unknown_(
        self
    ):
        """
        Detailed description.
        """
        ret = {}
        def rec(reader,lkey):
            ret[lkey] = reader.unknown()
            for key in reader:
                rec(reader[key],lkey+"."+key)
        for key in self.__readers:
            rec(self.__readers[key],key)
        return ret


    def __writeAll_(
        self
    ):
        """
        Detailed description.
        """
        count = 0
        for (path,block) in self.__paths:
            if not pathExists(dirname(path)):
                makedirs(dirname(path))
            writer = self._writer_(path,block)
            if not isinstance(reader,AbstractWriter):
                raise ScanError("Returned object is not an abstract writer.")
            rp = open(pathJoin(self.__rootPath,path),"r")
            new = "\n".join(writer()) + "\n"
            if pathExists(path):
                old = open(rp,"r").read()
                if old != new:
                    open(rp,"w").write(new)
            else:
                open(rp,"w").write(new)
            count += 1
            self.__update(50 + count*50/len(self.__paths))
