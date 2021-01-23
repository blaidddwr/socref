"""
Contains the ParserModel class.
"""
import traceback
from PySide2 import QtCore as qtc
from ._abstractparser import AbstractParser




class ParserModel(qtc.QObject):
    """
    This is the singleton parser model class. It handles execution of a given
    abstract parser. A slot is provided for starting a new abstract parser.
    Signals are provided for informing when parsing begins, makes progress,
    finishes, and has remaining unknown code fragments. This class is designed
    to run on its own thread because parsing can be a long process. Because of
    this the GUI must interface with this class exclusively through its signals
    and slots.
    """


    #
    # Signals this parser has finished parsing.
    #
    finished = qtc.Signal()


    #
    # Signals this parser has made the given percentage progress parsing. The
    # range given is from 0 to 100.
    #
    progressed = qtc.Signal(int)


    #
    # Signals this parser had remaining unknown code fragments after finishing
    # the last parsing.
    #
    remained = qtc.Signal(dict)


    #
    # Signals this parser has started parsing.
    #
    started = qtc.Signal()


    def __init__(
        self
    ):
        """
        Initializes a new parser.
        """
        super().__init__()
        self.__progress = 0


    @qtc.Slot(AbstractParser)
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
        parser : socref.abstract.AbstractParser
                 The abstract parser that is executed.
        """
        self.__progress = 0
        self.started.emit()
        try:
            parser.parse(self.__update_)
            unknown = parser.unknown()
            if unknown:
                self.remained.emit(unknown)
        except Exception as ok:
            traceback.print_exc()
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
