"""
Contains the ReaderBase class.
"""
from ..Abstract.AbstractParser import *
from ..Abstract.AbstractReader import *
from ..Error.ScanError import *




class ReaderBase(AbstractReader):
    """
    This is the reader base class. It partially implements the abstract reader
    class. This base class handles the saving of its parent parser or reader,
    all file access interfaces that call their parent parser's methods of the
    same name, setting its key, getting its key, and the main call operator
    interface for parsing.
    """


    def __init__(
        self
        ,parent
    ):
        """
        Initializes this new reader base instance with the given parent.

        Parameters
        ----------
        parent : object
                 The parent of this new abstract reader. If this is a root
                 reader the parent must be the parser that created it, else the
                 parent must be the parent reader that created this new child
                 reader.
        """
        super().__init__()
        if isinstance(parent,AbstractReader):
            self.__parser = parent.__parser
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
        self.__parser.discard()


    def key(
        self
    ):
        if self.__key is None:
            raise ScanError("Cannot return reader key not yet set.")
        return self.__key


    def read(
        self
    ):
        return self.__parser.read()


    def restore(
        self
    ):
        self.__parser.restore()


    def save(
        self
    ):
        self.__parser.save()


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
