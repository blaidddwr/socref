"""
Contains the WriterBase class.
"""
from ..Abstract.AbstractParser import *
from ..Abstract.AbstractWriter import *
from ..Error.ScanError import *




class WriterBase(AbstractWriter):
    """
    This is the writer base class. It partially implements the abstract writer
    class. This base class handles saving its parent parser or writer, the main
    call operator interface for returning all output, and reader lookup.
    """


    def __init__(
        self
        ,parent
    ):
        """
        Initializes this new writer base instance with the given parent.

        Parameters
        ----------
        parent : object
                 The parent of this new abstract writer. If this is a root
                 writer then the parent must be the parser that created it, else
                 the parent must be the parent writer that created this new
                 child writer.
        """
        super().__init__()
        if isinstance(parent,AbstractWriter):
            self.__parser = parent.__parser
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
        return self.__parser.lookup(key)
