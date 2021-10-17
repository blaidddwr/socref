"""
Contains the WriterBase class.
"""
from ..Abstract.AbstractParser import AbstractParser
from ..Abstract.AbstractWriter import AbstractWriter
from ..Error.ScanError import ScanError
from abc import abstractmethod




class WriterBase(AbstractWriter):
    """
    This is the writer base class. It partially implements the abstract writer
    class. This base class handles its parent parser and the main call operator
    interface for returning all output.

    An interface for generating header code before children output and
    generating footer code after children output is provided. These interfaces
    are broken sub tasks of the main call operator interface implemented by this
    base class where all output is returned.
    """


    def __init__(
        self
        ,parent
    ):
        """
        Initializes this new writer base instance with the given parent. The
        parent is a parser if this is the root writer else it is another writer
        which created this new writer as its child.

        Parameters
        ----------
        parent : object
                 The parent.
        """
        super().__init__()
        if isinstance(parent,AbstractWriter):
            self.__parser = parent.parent()
            parent.__children.append(self)
        elif isinstance(parent,AbstractParser):
            self.__parser = parent
        else:
            raise ScanError("Given parent is not an abstract parser or writer.")
        self.__children = []


    def __call__(
        self
    ):
        ret = list(self._header_())
        for child in self.__children:
            ret += child()
        ret += self._footer_()
        return ret


    def lookup(
        self
        ,key
    ):
        """
        Calls this reader's shared parser instance method with the same name.

        Parameters
        ----------
        key : object
              See parser method.
        """
        return self.__parser.lookup(key)


    def parent(
        self
    ):
        return self.__parser


    def _footer_(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : list
                 Footer output code lines of this writer, added after any
                 children writer code line output.
        """
        return []


    def _header_(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : list
                 Footer output code lines of this writer, added after any
                 children writer code line output.
        """
        return []
