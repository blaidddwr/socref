"""
Contains the LangController class.
"""
from ...Error.LangError import LangError
from ..Factory.BlockFactory import BlockFactory
from ..Factory.ParserFactory import ParserFactory
from importlib import import_module




class LangController():
    """
    This is the singleton language controller class. This controls the loading
    of new languages by preparing the required factories and importing the
    language's package or module.
    """
    __instance = None


    def load(
        self
        ,language
        ,name
    ):
        """
        Loads a new language with the given language name for this application
        by importing its package or module with the given name. The given
        language name must be unique among any other loaded language.

        Parameters
        ----------
        language : string
                   The language name.
        name : string
               The package or module name.
        """
        if language in BlockFactory.s():
            raise LangError("Language already loaded with the same name.")
        ParserFactory.s().beginRegistration(language)
        BlockFactory.s().beginRegistration(language)
        import_module(name)
        BlockFactory.s().endRegistration()
        ParserFactory.s().endRegistration()
        if not language in BlockFactory.s():
            raise LangError("Language did not register a root block.")
        if not language in ParserFactory.s():
            raise LangError("Language did not register a parser.")


    @classmethod
    def s(
        cls
    ):
        """
        Getter method.

        Returns
        -------
        instance : LangController
                   The singleton instance of this class.
        """
        if not cls.__instance:
            cls.__instance = LangController()
        return cls.__instance
