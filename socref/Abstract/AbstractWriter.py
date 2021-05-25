"""
Contains the AbstractWriter class.
"""
from abc import ABC
from abc import abstractmethod




class AbstractWriter(ABC):
    """
    This is the abstract writer class. A writer is a process interface that
    writes new output of a source code file. Writers can create additional
    writers as children during initialization whose output is also added to the
    source code file output, after the parent writer's header and before its
    footer. This allows modular object oriented design where each writer can be
    responsible for a specific type of code output. This class can lookup
    readers from its parent parser. The main call operator interface is used for
    generating output which it returns as iterable lines of code. The helper
    code output class provided by the core application can help in this process.

    An interface for generating header code before children output and
    generating footer code after children output is provided. These interfaces
    are broken sub tasks of the main call operator interface where all output is
    returned.
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
        This interface is a getter method.

        Returns
        -------
        result : lines
                 Output code lines written to this writer's source code file. If
                 this is a root writer then this is the full code output, else
                 if this is a child it is a code fragment added after its
                 parent's header and before its footer.
        """
        pass


    @abstractmethod
    def lookup(
        self
        ,key
    ):
        """
        This interface calls this reader's shared parser instance method with
        the same name.

        Parameters
        ----------
        key : object
              See parser method.
        """
        pass


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
