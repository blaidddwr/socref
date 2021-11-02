"""
Contains the ParseController class.
"""
from ...Abstract.AbstractParser import AbstractParser
from PySide6.QtCore import (
    QObject
    ,Signal
    ,Slot
)
from traceback import print_exc




class ParseController(QObject):
    """
    This is the singleton parse controller class. This controls the execution of
    a given abstract parser. A slot is provided for starting a new parser.
    Signals are provided for status changes of this controller. This singleton
    class is in its own thread to not block the main GUI thread when a parser is
    executing.
    """
    __instance = None


    #
    # Signals this controller has finished running its current parser.
    #
    finished = Signal()


    #
    # Signals this controller's parser has made the given percentage progress
    # parsing. The range given is from 0 to 100.
    #
    progressed = Signal(int)


    #
    # Signals this controller's parser had remaining unknown code fragments
    # after finishing. The code fragments are stored in a dictionary, where the
    # values are lists of code line fragments.
    #
    remained = Signal(dict)


    #
    # Signals this controller has started a new parser.
    #
    started = Signal()


    def __init__(
        self
    ):
        super().__init__()
        self.__progress = 0


    @classmethod
    def s(
        cls
    ):
        """
        Getter method.

        Returns
        -------
        instance : ParseController
                   The singleton instance of this class.
        """
        if not cls.__instance:
            cls.__instance = ParseController()
        return cls.__instance


    @Slot(AbstractParser)
    def start(
        self
        ,parser
    ):
        """
        Called to start execution of the given abstract parser, returning when
        execution is complete. Any exception thrown while executing the given
        parser is caught and printed to standard output.

        Parameters
        ----------
        parser : AbstractParser
                 The abstract parser.
        """
        self.__progress = 0
        self.started.emit()
        try:
            unknown = parser(self.__update_)
            if unknown:
                self.remained.emit(unknown)
        except Exception as ok:
            print_exc()
        finally:
            self.finished.emit()


    def __update_(
        self
        ,percent
    ):
        """
        Called by the currently executing abstract parser of this controller to
        inform this controller that progress of parsing has changed to the given
        percentage ranging from 0 to 100.

        Parameters
        ----------
        percent : int
                  The percentage.
        """
        if percent > self.__progress:
            self.__progress = percent
            self.progressed.emit(percent)
