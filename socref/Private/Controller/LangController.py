"""
Contains the LangController class.
"""
from ...Error.LangError import *
from ..Factory import blockFactory
from ..Factory import parserFactory
from importlib import import_module




class LangController():
    """
    This is the singleton language controller class. This controls the loading
    of new languages by preparing the required factories and importing the
    language's package or module.
    """


    def load(
        self
        ,language
        ,name
    ):
        """
        Loads a new language for this application by importing its package or
        module with the given name. The given language name must be unique among
        any other loaded language.

        Parameters
        ----------
        language : string
                   The language's name that is loaded.
        name : string
               The name of the language's package or module that is imported.
        """
        if language in blockFactory:
            raise LangError("Language already loaded with the same name.")
        parserFactory.beginRegistration(language)
        blockFactory.beginRegistration(language)
        import_module(name)
        blockFactory.endRegistration()
        parserFactory.endRegistration()
        if not language in blockFactory:
            raise LangError("Language did not register a root block.")
        if not language in parserFactory:
            raise LangError("Language did not register a parser.")
