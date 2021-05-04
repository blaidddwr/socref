"""
Contains the Socrates' Reference application.
"""
from . import resources


def block(
    name
    ,root=False
):
    """
    Decorates a class by registering it as a block with the given type name for
    the language implementation currently being imported by the application.

    Parameters
    ----------
    name : string
           The type name of the decorated class that is registered as a block.
    root : bool
           True to register the decorated class as the root block or false
           otherwise. Only one block class can be registered as root for each
           imported language.
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
    class_ : socref.Abstract.AbstractParser
             The decorated class that is registered.
    """
    parserFactory.register(class_)
    return class_


VERSION = "0.0.9999"
DICTIONARY = "en_US"
HUNSPELL_ROOT = "/usr/share/hunspell"


from .Private.Factory import blockFactory
from .Private.Factory import parserFactory
