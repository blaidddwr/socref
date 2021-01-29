"""
Contains the ParseController class.
"""
from ....Abstract.AbstractParser import *
from PySide2.QtCore import QObject
from PySide2.QtCore import Signal
from PySide2.QtCore import Slot
from traceback import print_exc




class ParseController(QObject):
    """
    This is the singleton parser model class. It handles execution of a given
    abstract parser. A slot is provided for starting a new abstract parser.
    Signals are provided for informing when parsing begins, makes progress,
    finishes, and has remaining unknown code fragments. This class is designed
    to run on its own thread because parsing can be a long process. Because of
    this the GUI must interface with this class exclusively through its signals
    and slots.

    _DEPCRECATED_
    """


    #
    # Signals this parser has finished parsing.
    #
    finished = Signal()


    #
    # Signals this parser has made the given percentage progress parsing. The
    # range given is from 0 to 100.
    #
    progressed = Signal(int)


    #
    # Signals this parser had remaining unknown code fragments after finishing
    # the last parsing.
    #
    remained = Signal(dict)


    #
    # Signals this parser has started parsing.
    #
    started = Signal()


    def __init__(
        self
    ):
        """
        Initializes a new parser.
        """
        super().__init__()
        self.__progress = 0


    @Slot(AbstractParser)
    def start(
        self
        ,parser
    ):
        """
        Called to start execution of the given abstract parser, returning when
        execution is complete.

        This also catches any python exceptions and prints them to standard
        error because Qt thread's event loop ignores them.

        Parameters
        ----------
        parser : socref.Abstract.AbstractParser
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
        Called by the abstract parser that this parser model is currently
        parsing to inform this parser that progress of the given percentage has
        been made in parsing.

        Parameters
        ----------
        percent : int
                  The percentage progress made by this parser model's abstract
                  parser ranging from 0 to 100.
        """
        if percent > self.__progress:
            self.__progress = percent
            self.progressed.emit(percent)
