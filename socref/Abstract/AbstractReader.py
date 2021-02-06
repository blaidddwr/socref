"""
Contains the AbstractReader class.
"""
from . import AbstractParser
from ..Error.ScanError import *
from abc import ABC
from abc import abstractmethod




class AbstractReader(ABC):
    """
    This is the abstract reader class. A reader is a process interface that
    scans a source code file or part of a currently scanned source code file.
    While a reader implementation is scanning a file it can create additional
    child readers for specialized scanning. This allows modular object oriented
    design where each reader class can be responsible for a specific type of
    code.

    A reader implementation must set its key before the end of its scanning
    interface. The key is used to register every created reader, including child
    readers, to the shared parent parser in its reader lookup table. In this way
    readers can be accessed by writers to get the scanned lines of code that are
    preserved in output.

    A reader implementation returns any unused lines of code after parsing and
    all writers have been processed. In this way the user can be notified of any
    scanned code lines that will be discarded and recover those lines if it was
    a mistake. Because of this interface a reader must expunge any code lines
    used by a writer so they are not returned as unused code.

    Methods are provided for accessing the source code file being read by the
    shared parent parser. Because all child readers share the same singular
    parser they all share the same file, cursor position, and saved cursor
    position stack.
    """


    def __init__(
        self
        ,parent
    ):
        """
        Initializes this new abstract reader with the given parent.

        Parameters
        ----------
        parent : object
                 The parent of this new abstract reader. If this is a root
                 reader the parent must be the parser that created it, else the
                 parent must be the parent reader that created this new child
                 reader.
        """
        if isinstance(parent,AbstractReader):
            self.__parser = parent.__parser
        elif isinstance(parent,AbstractParser.AbstractParser):
            self.__parser = parent
        else:
            raise ScanError("Given parent is not an abstract parser or reader.")
        self.__key = None


    def __call__(
        self
    ):
        """
        Reads this reader's shared parent parser's open source code file by
        scanning it, creating any necessary child readers that also read, and
        then setting its key.
        """
        self._scan_()
        if self.__key is None:
            raise ScanError("An abstract reader failed to set its key after scanning.")
        self.__parser.addLookup(self.__key,self)


    def discard(
        self
    ):
        """
        Discards the last file cursor position saved in this parent parser's
        current read file.
        """
        self.__parser.discard()


    def key(
        self
    ):
        """
        Getter method. This reader's key must be set.

        Returns
        -------
        result : string
                 This reader's unique key used to identify itself in its parent
                 parser's reader lookup table.
        """
        if self.__key is None:
            raise ScanError("Cannot return reader key not yet set.")
        return self.__key


    def read(
        self
    ):
        """
        Detailed description.
        """
        return self.__parser.read()


    def restore(
        self
    ):
        """
        Detailed description.
        """
        self.__parser.restore()


    def save(
        self
    ):
        """
        Detailed description.
        """
        self.__parser.save()


    @abstractmethod
    def unknown(
        self
    ):
        """
        Detailed description.
        """
        pass


    @abstractmethod
    def _scan_(
        self
    ):
        """
        Detailed description.
        """
        pass


    def _setKey_(
        self
        ,key
    ):
        """
        Detailed description.

        Parameters
        ----------
        key : 
        """
        if self.__key is not None:
            raise ScanError("An abstract reader attempted to overwrite its key.")
        self.__key = key
