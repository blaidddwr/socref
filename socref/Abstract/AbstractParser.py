"""
Contains the AbstractParser class.
"""
from abc import ABC
from abc import abstractmethod




class AbstractParser(ABC):
    """
    This is the abstract parser class. A parser is a controller interface that
    controls the parsing of a project's source code. It provides a path list of
    all code files to be processed, optional abstract readers to read in each
    processed path, and abstract writers to output to each path. The path list
    is a list of tuples, each tuple containing a relative path and the block
    associated with it. This class provides interfaces for accessing the current
    file being read, but should never be accessed directly. The abstract reader
    and writer classes provide all methods either needs from the parser which
    created them.

    An instance of this class begins parsing by calling its call operator, where
    it goes through the generated path list and return any unknown lines of code
    parsed but not used. For each path it reads, creates, and writes to each
    path as necessary using the reader and writer interfaces to generate the
    appropriate instances of both. Interfaces are provided to adding to and
    getting from a reader lookup table for all readers created while parsing
    existing paths. This class provides an interface for setting the root path
    used to get the absolute path of each relative path in the path list.

    An implemented parser class must have one initialization argument, that
    argument being the root block of the project that is being parsed. From this
    root block of a project, it is expected to be able to generate the path
    list.

    An interface for generating the path list, creating readers for a given path
    and block, and creating writers for a given path and block are provided for
    convenience. All these interfaces are broken down sub tasks of the main call
    operator interface.
    """


    def __init__(
        self
    ):
        super().__init__()


    @abstractmethod
    def __call__(
        self
        ,update
    ):
        """
        This interface parses all source code files of the project of this
        parser's root block, updating its progress with the given callback
        object. The root path of this parser must be set. This parser's reader
        lookup table is cleared after parsing is complete or fails.

        Parameters
        ----------
        update : function
                 Used to update the progress of this scan. It takes one argument
                 that is the progress as a percentage from 1 to 99.
        """
        pass


    @abstractmethod
    def addLookup(
        self
        ,key
        ,reader
    ):
        """
        This interface adds the given reader to this parser's reader lookup
        table with the given key.

        Parameters
        ----------
        key : string
              Key used to insert the given reader into this parser's reader
              lookup table. It must not already exist as a key in the table.
        reader : socref.Abstract.AbstractReader
                 A reader that is inserted into this parser's reader lookup
                 table.
        """
        pass


    @abstractmethod
    def discard(
        self
    ):
        """
        This interface discards the last file cursor position saved in this
        parser's current read file. This parser must be reading a file and have
        at least one cursor state on its saved stack.
        """
        pass


    @abstractmethod
    def lookup(
        self
        ,key
    ):
        """
        This interface is a getter method.

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
        pass


    @abstractmethod
    def read(
        self
    ):
        """
        This interface reads the next line of this parser's active reading file,
        moving its cursor to the next line. This parser must be reading a file.

        Returns
        -------
        spaces : int
                 The total number of spaces of the read line or None if the end
                 of file has been reached.
        line : string
               The read line stripped of any white space from both sides or None
               if the end of file has been reached.
        """
        pass


    @abstractmethod
    def restore(
        self
    ):
        """
        This interface restores the last file cursor position saved in this
        parser's current read file, removing it from the saved stack. This
        parser must be reading a file and have at least one cursor state on its
        saved stack.
        """
        pass


    @abstractmethod
    def save(
        self
    ):
        """
        This interface saves the current cursor position of this parser's
        current read file, adding it to the save stack. This parser must be
        reading a file.
        """
        pass


    @abstractmethod
    def setRootPath(
        self
        ,path
    ):
        """
        This interface sets the root path of this parser. This can only be
        called once when this parser's root path is empty. This must be called
        before parsing begins.

        Parameters
        ----------
        path : string
               The root path of this parser.
        """
        pass


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
