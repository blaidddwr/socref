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
        BlockFactory.s().register(class_,name)
        if root:
            BlockFactory.s().registerRoot(class_)
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
    ParserFactory.s().register(class_)
    return class_


from .Private.Factory.BlockFactory import BlockFactory
from .Private.Factory.ParserFactory import ParserFactory
