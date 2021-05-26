"""
Contains the BlockFactory class.
"""
from ...Abstract.AbstractBlock import *
from ...Error.LangError import *
from ...Error.RegisterError import *




class BlockFactory():
    """
    This is the singleton block factory class. It registers block classes for a
    new language and creates new blocks of registered languages from the
    registered classes. Languages are registered using a unique name. Block
    classes are registered using a unique name within its language. One and only
    one special root block class must be registered for each language.
    """


    def __init__(
        self
    ):
        self.__blocks = {}
        self.__rootBlocks = {}
        self.__rn = None
        self.__rl = None


    def __contains__(
        self
        ,language
    ):
        return language in self.__rootBlocks


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
        if language in self.__rootBlocks.keys():
            raise LangError("Language already registered with given name.")
        self.__blocks[language] = {}
        self.__rn = language
        self.__rl = self.__blocks[language]


    def create(
        self
        ,language
        ,name=None
    ):
        """
        Getter method. The given language name and block type name, if one is
        given, must exist in this factory.

        Parameters
        ----------
        language : string
                   The returned block's language name.
        name : object
               The returned block's type name string or None for the special
               root block.

        Returns
        -------
        result : AbstractBlock
                 A new block of the given type or a root block if no type is
                 given from the given language.
        """
        assert(self.__rn is None)
        if name is None:
            return self.__rootBlocks[language]()
        else:
            return self.__blocks[language][name]()


    def endRegistration(
        self
    ):
        """
        Ends registration of the language currently being registered. This must
        be called only during a registration process to end it.
        """
        assert(self.__rn is not None)
        if self.__rn not in self.__rootBlocks:
            del self.__blocks[self.__rn]
        self.__rn = None
        self.__rl = None


    def languages(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : list
                 Sorted language names this factory has loaded.
        """
        assert(self.__rn is None)
        ret = list(self.__rootBlocks.keys())
        ret.sort()
        return ret


    def register(
        self
        ,class_
        ,name
    ):
        """
        Registers the given block class with the given type name to the language
        currently being registered. The given block class is assigned special
        attributes _LANG_ and _TYPE_. This must be called during registration of
        a new language. The given type name must be unique within its language.

        Parameters
        ----------
        class_ : AbstractBlock
                 A class object that is registered as a block type of the
                 currently loading language.
        name : string
               The registered block's type name.
        """
        if self.__rn is None:
            raise LangError("Cannot register a block when no language is being registered.")
        if not issubclass(class_,AbstractBlock):
            raise RegisterError("Given object is not an abstract block class.")
        if not name[:1].isalpha():
            raise RegisterError("Block type name must begin with alphabetical character.")
        if not name.isalnum():
            raise RegisterError("Block type name must be alphanumeric.")
        if name in self.__rl:
            raise RegisterError("Block class is already registered with the same name.")
        self.__rl[name] = class_
        class_._LANG_ = self.__rn
        class_._TYPE_ = name


    def registerRoot(
        self
        ,class_
    ):
        """
        Registers the given block class as the root block type of the language
        currently being registered. Only one class object can be the root block
        of a language.

        Parameters
        ----------
        class_ : AbstractBlock
                 A class object that is registered as the root block type of the
                 currently loading language.
        """
        if self.__rn is None:
            raise LangError("Cannot register a block when no language is being registered.")
        if not issubclass(class_,AbstractBlock):
            raise RegisterError("Given object is not an abstract block class.")
        if self.__rn in self.__rootBlocks:
            print(self.__rootBlocks)
            raise RegisterError("Root block class is already registered for registering language.")
        self.__rootBlocks[self.__rn] = class_
