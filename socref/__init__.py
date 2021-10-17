"""
Contains the Socrates' Reference application.
"""
from . import resources


def block(
    name
    ,root=False
):
    """
    Decorates a class by registering it as a block with the given type name and
    optional root flag for the language implementation currently being imported
    by the application. Only one block can be registered as the root block for a
    language implementation.

    Parameters
    ----------
    name : string
           The type name.
    root : bool
           True to register as root or false otherwise.
    """
    def wrapper(class_):
        blockFactory.register(class_,name)
        if root:
            blockFactory.registerRoot(class_)
        return class_
    return wrapper


def parser(
    class_
):
    """
    Decorates a class by registering it as the parser for the language
    implementation currently being imported by this application. Only one parser
    class can be registered for each imported language.

    Parameters
    ----------
    class_ : AbstractParser
             The decorated class.
    """
    parserFactory.register(class_)
    return class_


VERSION = "0.0.9999"
DICTIONARY = "en_US"
HUNSPELL_ROOT = "/usr/share/hunspell"


from .Private.Factory import blockFactory
from .Private.Factory import parserFactory
