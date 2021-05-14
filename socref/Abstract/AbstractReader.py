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
        Discards the last file cursor position saved in the shared parser's
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
        Reads the next line of the shared parser's active reading file, moving
        its cursor to the next line.

        Returns
        -------
        spaces : int
                 The total number of spaces of the read line or None if the end
                 of file has been reached.
        line : string
               The read line stripped of any white space from both sides or None
               if the end of file has been reached.
        """
        return self.__parser.read()


    def restore(
        self
    ):
        """
        Restores the last file cursor position saved in the shared parser's
        current read file, removing it from the saved stack. The parser must
        have at least one cursor state on its saved stack.
        """
        self.__parser.restore()


    def save(
        self
    ):
        """
        Saves the current cursor position of the shared parser's current read
        file, adding it to the save stack.
        """
        self.__parser.save()


    @abstractmethod
    def unknown(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : list
                 Code lines that were not taken from this reader by any writers.
        """
        pass


    @abstractmethod
    def _scan_(
        self
    ):
        """
        This interface scans its parent parser's open source code file for any
        lines that need to be saved for writers. This method must set its
        reader's key before returning.
        """
        pass


    def _setKey_(
        self
        ,key
    ):
        """
        Sets this reader's key to the given value. A reader's key can only be
        set once.

        Parameters
        ----------
        key : string
              The new value of this reader's key.
        """
        if self.__key is not None:
            raise ScanError("An abstract reader attempted to overwrite its key.")
        self.__key = key
