"""
Contains the ParserFactory class.
"""
from ....Abstract.AbstractParser import *
from ....Error.LangError import *
from ....Error.RegisterError import *




class ParserFactory():
    """
    Detailed description.
    """


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
        Begins registration of a new language with the given name. The name must
        not already exist as a language. This cannot be called when registration
        of another language is ongoing.

        Parameters
        ----------
        language : string
                   The language's name that will begin registration.
        """
        if language in self.__parsers.keys():
            raise LangError("Language already registered with given name.")
        self.__rn = language


    def create(
        self
        ,block
    ):
        """
        Getter method.

        Parameters
        ----------
        block : socref.Abstract.AbstractBlock
                The root block of the project used to create the returned
                parser.

        Returns
        -------
        ret0 : socref.Abstract.AbstractParser
               A new parser from the project with the given root block.
        """
        assert(self.__rn is None)
        return self.__rootBlocks[block._LANG_](block)


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
        Registers the given parser class the language currently being
        registered.

        Parameters
        ----------
        class_ : socref.Abstract.AbstractParser
                 A class object that is registered as the parser of the
                 currently loading language.
        """
        if self.__rn is None:
            raise LangError("Cannot register a parser when no language is being registered.")
        if not issubclass(class_,AbstractParser):
            raise RegisterError("Given object is not an abstract parser class.")
        if self.__rn in self.__parsers:
            raise RegisterError("Parser class is already registered for registering language.")
        self.__parsers[self.__rn] = class_
