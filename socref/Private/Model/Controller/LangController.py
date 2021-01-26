"""
Contains the LangController class.
"""
from ....Error.LangError import *
from ..Factory import blockFactory
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
            raise exception.LangError("Language already loaded with the same name")
        # begin parser factory registration
        blockFactory.beginRegistration(language)
        import_module(import_name)
        blockFactory.endRegistration()
        # end parser factory registration
        if not language in blockFactory:
            raise exception.LangError("Language did not register a root block.")
        # make sure parser was registered
