"""
Contains the AbstractParser class.
"""
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


    def parse(
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
        try:
            self.__update = update
            self.__paths = self._pathList_()
            if len(set(self.__paths)) != len(self.__paths):
                raise ScanError("Duplicate file names generated for parsing. This is caused by two"
                                " blocks generating an identical file name. Perhaps check for"
                                " blocks with the same display name and parent block.")
            self.__readAll_()
            self.__writeAll_()
        finally:
            self.__update = None
            self.__paths = []
            self.__readers = {}


    def peakLine(
        self
    ):
        """
        Detailed description.
        """
        self.save()
        ret = self.readLine()
        self.restore()
        return ret


    def readLine(
        self
    ):
        """
        Detailed description.
        """
        if self.__io is None:
            raise ScanError()!!!!!!!!!!
        line = self.__io.readline()
        if not line:
            return None
        indent = len(line)-len(line.lstrip(' '))
        return (indent,line.strip())


    def restore(
        self
    ):
        """
        Detailed description.
        """
        if self.__io is None:
            raise ScanError()!!!!!!!!!!
        if not self.__stack:
            raise ScanError()!!!!!!!!!!
        self.__io.seek(self.__stack.pop())


    def save(
        self
    ):
        """
        Detailed description.
        """
        if self.__io is None:
            raise ScanError()!!!!!!!!!!
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


    def __readAll_(
        self
    ):
        """
        Detailed description.
        """
        count = 0
        for path in self.__paths:
            if pathExists(path):
                self.__io = open(pathJoin(self.__rootPath,path),"r")
                try:
                    self.__ifile = ifile
                    reader = self._rootReader_(path)
                    reader()
                    if reader.key() in self.__readers:
                        raise ScanError()!!!!!!!!
                    self.__readers[reader.key()] = reader
                finally:
                    self.__io = None
            count += 1
            self.__update(count*50/len(self.__paths))


    def __writeAll_(
        self
    ):
        """
        Detailed description.
        """
        pass
