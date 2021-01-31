"""
Contains the core application for Socrates' Reference.
"""
from . import resources


def block(
    name
    ,root=False
):
    """
    Detailed description.

    Parameters
    ----------
    name : 
    root : 
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
    class_ : 
    """
    parserFactory.register(class_)
    return class_


VERSION = "0.0.9999"
DICTIONARY = "en_US"
HUNSPELL_ROOT = "/usr/share/hunspell"


from .Private.Factory import blockFactory
from .Private.Factory import parserFactory
