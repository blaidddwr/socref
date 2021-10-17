"""
Contains the ReaderBase class.
"""
from ..Abstract.AbstractParser import AbstractParser
from ..Abstract.AbstractReader import AbstractReader
from ..Error.ScanError import ScanError
from abc import abstractmethod




class ReaderBase(AbstractReader):
    """
    This is the reader base class. It partially implements the abstract reader
    class. This base class handles the saving of its parent parser or reader,
    all file access interfaces that call their parent parser's methods of the
    same name, setting its key, getting its key, and the main call operator
    interface for parsing. It provides methods for accessing the currently open
    file of its parent parser with methods of the same name.

    An interface for parsing this parent parser's currently open file is
    provided for convenience. This interface is a sub task of the main call
    operator implemented by this base class, and must set this reader's key
    before returning.
    """


    def __init__(
        self
        ,parent
    ):
        """
        Initializes this new reader base instance with the given parent. The
        parent is a parser if this is the root reader else it is another reader
        which created this new reader as its child.

        Parameters
        ----------
        parent : object
                 The parent.
        """
        super().__init__()
        if isinstance(parent,AbstractReader):
            self.__parser = parent.parent()
        elif isinstance(parent,AbstractParser):
            self.__parser = parent
        else:
            raise ScanError("Given parent is not an abstract parser or reader.")
        self.__key = None


    def __call__(
        self
    ):
        self._scan_()
        if self.__key is None:
            raise ScanError("An abstract reader failed to set its key after scanning.")
        self.__parser.addLookup(self.__key,self)


    def discard(
        self
    ):
        """
        Calls this reader's shared parser instance method with the same name.
        """
        self.__parser.discard()


    def key(
        self
    ):
        if self.__key is None:
            raise ScanError("Cannot return reader key not yet set.")
        return self.__key


    def parent(
        self
    ):
        return self.__parser


    def read(
        self
    ):
        """
        Calls this reader's shared parser instance method with the same name.
        """
        return self.__parser.read()


    def restore(
        self
    ):
        """
        Calls this reader's shared parser instance method with the same name.
        """
        self.__parser.restore()


    def save(
        self
    ):
        """
        Calls this reader's shared parser instance method with the same name.
        """
        self.__parser.save()


    @abstractmethod
    def _scan_(
        self
    ):
        """
        This interface parses its parent parser's currently open code file for
        anything that needs to be saved for writers. This method must set its
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
              The key.
        """
        if self.__key is not None:
            raise ScanError("An abstract reader attempted to overwrite its key.")
        self.__key = key
