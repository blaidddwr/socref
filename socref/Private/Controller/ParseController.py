"""
Contains the ParseController class.
"""
from ...Abstract.AbstractParser import *
from PySide6.QtCore import QObject
from PySide6.QtCore import Signal
from PySide6.QtCore import Slot
from traceback import print_exc




class ParseController(QObject):
    """
    This is the singleton parse controller class. This controls the execution of
    a given abstract parser. A slot is provided for starting a new parser.
    Signals are provided for status changes of this controller. This singleton
    class is in its own thread to not block the main GUI thread when a parser is
    executing.
    """


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
                 The abstract parser that is executed.
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
        percentage.

        Parameters
        ----------
        percent : int
                  The percentage progress ranging from 0 to 100.
        """
        if percent > self.__progress:
            self.__progress = percent
            self.progressed.emit(percent)
