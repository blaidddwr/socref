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
    source code file output. This allows modular object oriented design where
    each writer can be responsible for a specific type of code output. The main
    call operator interface is used for generating output which it returns as
    iterable lines of code.
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
    def parent(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : AbstractParser
                 The shared parent parser of this writer.
        """
        pass
