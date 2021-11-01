"""
Contains the ParserFactory class.
"""
from ...Abstract.AbstractParser import AbstractParser
from ...Error.LangError import LangError
from ...Error.RegisterError import RegisterError




class ParserFactory():
    """
    This is the singleton parser factory. It registers a parser class
    implementation for a new language and creates new parser instances on demand
    for any given language. Every new language must register one and only one
    parser implementation.
    """
    __instance = None


    def __init__(
        self
    ):
        self.__parsers = {}
        self.__rn = None


    def __contains__(
        self
        ,language
    ):
        return language in self.__parsers


    def beginRegistration(
        self
        ,language
    ):
        """
        Begins registration of a new language with the given language name. The
        given language name must not already exist as a language. This cannot be
        called when registration of another language is ongoing.

        Parameters
        ----------
        language : string
                   The language name.
        """
        if language in self.__parsers.keys():
            raise LangError("Language already registered with given name.")
        self.__rn = language


    def create(
        self
        ,block
    ):
        """
        Creates and returns a new parser from the project with the given root
        block.

        Parameters
        ----------
        block : AbstractBlock
                The root block.

        Returns
        -------
        result : AbstractParser
                 The new parser.
        """
        assert(self.__rn is None)
        return self.__parsers[block._LANG_](block)


    def endRegistration(
        self
    ):
        """
        Ends registration of the language currently being registered. This must
        be called only during a registration process to end it.
        """
        assert(self.__rn is not None)
        self.__rn = None


    def register(
        self
        ,class_
    ):
        """
        Registers the given parser class for the language currently being
        registered.

        Parameters
        ----------
        class_ : AbstractParser
                 The parser class.
        """
        if self.__rn is None:
            raise LangError("Cannot register a parser when no language is being registered.")
        if not issubclass(class_,AbstractParser):
            raise RegisterError("Given object is not an abstract parser class.")
        if self.__rn in self.__parsers:
            raise RegisterError("Parser class is already registered for registering language.")
        self.__parsers[self.__rn] = class_


    @classmethod
    def s(
        cls
    ):
        """
        Getter method.

        Returns
        -------
        instance : ParserFactory
                   The singleton instance of this class.
        """
        if not cls.__instance:
            cls.__instance = ParserFactory()
        return cls.__instance
