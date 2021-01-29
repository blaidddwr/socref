"""
Contains the core application for Socrates' Reference.
"""
from . import resources
from .Private.Factory import blockFactory
from .Private.Factory import parserFactory


def block(
    name
    ,root=False
):
    """
    Detailed description.

    Parameters
    ----------
    name : object
           Detailed description.
    root : object
           Detailed description.
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
    Detailed description.

    Parameters
    ----------
    class_ : object
             Detailed description.
    """
    parserFactory.register(class_)
    return class_


VERSION = "0.0.9999"
DICTIONARY = "en_US"
HUNSPELL_ROOT = "/usr/share/hunspell"
