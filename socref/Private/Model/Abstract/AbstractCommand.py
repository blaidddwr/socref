"""
Contains the AbstractCommand class.
"""
from abc import ABC
from abc import abstractmethod




class AbstractCommand(ABC):
    """
    This is the abstract command class. A command is an action interface
    designed to redo or undo a single action upon a project model.
    """


    def __init__(
        self
    ):
        super().__init__()


    @abstractmethod
    def redo(
        self
    ):
        """
        This interface acts on this command's project model to redo its command.
        """
        pass


    @abstractmethod
    def undo(
        self
    ):
        """
        This interface acts on this command's project model to undo its command.
        """
        pass
