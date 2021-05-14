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
    This is the abstract parser class. A parser is a controller interface that
    controls the parsing of a project's source code. A parser's interface
    provides a path list of all code files to be processed, abstract readers to
    read in each processed path, and abstract writers to output the parsed
    results of each processed path. Each path must have an abstract writer, but
    an abstract reader is optional. The path list is a list of tuples, each
    tuple containing a relative path and the block associated with it. The
    abstract parser class itself provides methods for accessing the current file
    being read, but should never be accessed directly. The abstract reader and
    writer classes provide all methods either needs from the parser which
    created them.

    An implemented parser class must have one initialization argument, that
    argument being the root block of the project that is being parsed. From this
    root block of a project it is expected to be able to generate the path list,
    and from each block within the tuple of the path list it is expected to
    generate the correct reader and writer.

    A parser returns any unknown lines of code after parsing is complete. This
    process is fully implemented by the abstract parser itself.
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
        Parses all source code files of the project of this parser's root block,
        updating its progress with the given callback object. The root path of
        this parser must be set. This parser's reader lookup table is cleared
        after parsing is complete or fails.

        Parameters
        ----------
        update : function
                 Used to update the progress of this scan. It takes one argument
                 that is the progress as a percentage from 1 to 99.
        """
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
        """
        Adds the given reader to this parser's reader lookup table with the
        given key.

        Parameters
        ----------
        key : string
              Key used to insert the given reader into this parser's reader
              lookup table. It must not already exist as a key in the table.
        reader : socref.Abstract.AbstractReader
                 A reader that is inserted into this parser's reader lookup
                 table.
        """
        assert(isinstance(reader,AbstractReader))
        if key in self.__readers:
            raise ScanError("An abstract reader set a duplicate key.")
        self.__readers[key] = reader


    def discard(
        self
    ):
        """
        Discards the last file cursor position saved in this parser's current
        read file. This parser must be reading a file and have at least one
        cursor state on its saved stack.
        """
        if self.__io is None:
            raise ScanError("Parser cannot discard without open file.")
        if not self.__stack:
            raise ScanError("Parser cannot discard when position stack is empty.")
        self.__stack.pop()


    def lookup(
        self
        ,key
    ):
        """
        Getter method.

        Parameters
        ----------
        key : string
              The matching key, if any, of the returned reader.

        Returns
        -------
        result : socref.Abstract.AbstractReader
                 The parser in this parser's reader lookup table with the given
                 key or none if there is no such reader with the given key.
        """
        return self.__readers.get(key,None)


    def read(
        self
    ):
        """
        Reads the next line of this parser's active reading file, moving its
        cursor to the next line. This parser must be reading a file.

        Returns
        -------
        spaces : int
                 The total number of spaces of the read line or None if the end
                 of file has been reached.
        line : string
               The read line stripped of any white space from both sides or None
               if the end of file has been reached.
        """
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
        """
        Restores the last file cursor position saved in this parser's current
        read file, removing it from the saved stack. This parser must be reading
        a file and have at least one cursor state on its saved stack.
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
        Saves the current cursor position of this parser's current read file,
        adding it to the save stack. This parser must be reading a file.
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
        this parser's root path is empty. This must be called before parsing
        begins.

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
        This interface is a getter method.

        Returns
        -------
        result : list
                 A list of tuples, each tuple containing a relative path to a
                 source code file that is parsed and the block associated with
                 it. The path is relative to the root path of the project being
                 parsed.
        """
        pass


    @abstractmethod
    def _reader_(
        self
        ,block
    ):
        """
        This interface is a getter method.

        Parameters
        ----------
        block : socref.Abstract.AbstractBlock
                The block associated with the source code file.

        Returns
        -------
        result : socref.Abstract.AbstractReader
                 A new reader capable of reading any lines of code that is saved
                 from the source code file associated with the given block. None
                 can be returned, in which case this parser ignores it and any
                 existing file, continuing to the next path.
        """
        pass


    @abstractmethod
    def _writer_(
        self
        ,block
    ):
        """
        This interface is a getter method.

        Parameters
        ----------
        block : socref.Abstract.AbstractBlock
                The block associated with the source code file.

        Returns
        -------
        result : socref.Abstract.AbstractWriter
                 A new writer capable of writing the full output of the source
                 code file associated with the given block.
        """
        pass


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
