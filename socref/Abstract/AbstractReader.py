"""
Contains the AbstractReader class.
"""
from abc import ABC
from abc import abstractmethod




class AbstractReader(ABC):
    """
    This is the abstract reader class. A reader is a process interface that
    scans a source code file or part of a currently scanning source code file of
    its parent parser. While this class is scanning a file it can create
    additional child readers for specialized scanning all sharing the same
    parent parser and its open file. This allows modular object oriented design
    where each reader class can be responsible for a specific type of code.

    The call operator interface is used to begin scanning, and a unique key must
    be set before the call operator returns. This unique key is used to register
    every created reader to he shared parent parser in its lookup table. This
    class returns any unused lines of code that were not gotten by a writer. It
    provides interfaces for accessing the currently open file of its parent
    parser with methods of the same name.

    An interface for parsing this parent parser's currently open file is
    provided for convenience. This interface is a sub task of the main call
    operator, whose only final task after this sub task is to add itself to its
    parent parser's reader lookup table.
    """


    def __init__(
        self
    ):
        super().__init__()


    @abstractmethod
    def __call__(
        self
    ):
        """
        This interface parses this reader's shared parent parser's currently
        open code file, creating any necessary child readers that also parse,
        setting its key, and then adding this instance to its parent parser's
        lookup table.
        """
        pass


    @abstractmethod
    def discard(
        self
    ):
        """
        This interface calls this reader's shared parser instance method with
        the same name.
        """
        pass


    @abstractmethod
    def key(
        self
    ):
        """
        This interface is a getter method. This reader's key must be set.

        Returns
        -------
        result : string
                 This reader's unique key used to identify itself in its parent
                 parser's reader lookup table.
        """
        pass


    @abstractmethod
    def read(
        self
    ):
        """
        This interface calls this reader's shared parser instance method with
        the same name.
        """
        pass


    @abstractmethod
    def restore(
        self
    ):
        """
        This interface calls this reader's shared parser instance method with
        the same name.
        """
        pass


    @abstractmethod
    def save(
        self
    ):
        """
        This interface calls this reader's shared parser instance method with
        the same name.
        """
        pass


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
        This interface parses its parent parser's currently open code file for
        anything that needs to be saved for writers. This method must set its
        reader's key before returning.
        """
        pass
