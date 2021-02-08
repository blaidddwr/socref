"""
Contains the AbstractWriter class.
"""
from . import AbstractParser
from abc import ABC
from abc import abstractmethod




class AbstractWriter(ABC):
    """
    This is the abstract writer class. A writer is a process interface that
    writes new output of a source code file. Writers can create additional
    writers as children whose output is also added to the source code file
    output, after the parent writer's header and before its footer. This allows
    modular object oriented design where each writer can be responsible for a
    specific type of code output. All children writers must be added in the
    parent's initialization method.

    A method for looking up readers in all writer's shared parent parser is
    provided to allow writers the ability to take scanned code lines.
    """


    def __init__(
        self
        ,parent
    ):
        """
        Initializes this new abstract writer with the given parent.

        Parameters
        ----------
        parent : object
                 The parent of this new abstract writer. If this is a root
                 writer then the parent must be the parser that created it, else
                 the parent must be the parent writer that created this new
                 child writer.
        """
        if isinstance(parent,AbstractWriter):
            self.__parser = parent.__parser
            parent.__children.append(self)
        elif isinstance(parent,AbstractParser.AbstractParser):
            self.__parser = parent
        else:
            raise ScanError("Given parent is not an abstract parser or writer.")
        self.__children = []


    def __call__(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : lines
                 Output code lines written to this writer's source code file. If
                 this is a root writer then this is the full code output, else
                 if this is a child it is a code fragment added after its
                 parent's header and before its footer.
        """
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
        Getter method.

        Parameters
        ----------
        key : string
              The matching key, if any, of the returned reader.

        Returns
        -------
        result : socref.Abstract.AbstractReader
                 The parser in the shared parser's reader lookup table with the
                 given key or none if there is no such reader with the given
                 key.
        """
        return self.__parser.lookup(key)


    @abstractmethod
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
        pass


    @abstractmethod
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
        pass
