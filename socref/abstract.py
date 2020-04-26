"""
Contains all abstract classes used implement a language and a function for
registering its blocks.
"""
from . import core

from ._abstractblock import AbstractBlock
from ._abstractparser import AbstractParser




def register(
    name
    ,root=False
    ):
    """
    Registers the wrapped class object as a block with the given name for the
    language currently being loaded. The class object is also assigned special
    attributes _LANG_ and _TYPE_ that provide the language name and block name,
    respectively. This must be called when a language is being loaded.

    Parameters
    ----------
    name : string
           The block's type name that is being registered. This must be unique
           among all block names of any one language and only contain
           alphabetical and underscore characters. Underscore characters are
           replaced with spaces when displays the block type to the user.
    root : bool
           Optionally indicates the registered block is the root block of the
           language if set to true. Only one block type can be the root of a
           language.
    """
    def wrapper(class_):
        core.blockFactory.registerBlock(class_,name)
        if root:
            core.blockFactory.registerRootBlock(class_)
        return class_
    return wrapper
