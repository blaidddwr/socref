"""
Contains the BlockFactory class.
"""
from ....Abstract.AbstractBlock import *
from ....Error.LangError import *
from ....Error.RegisterError import *




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
        self.__langBlocks = {}
        self.__langRootBlocks = {}
        self.__rn = None
        self.__rl = None


    def __contains__(
        self
        ,language
    ):
        return language in self.__langRootBlocks


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
        assert(not language in self.__langRootBlocks.keys())
        self.__langBlocks[language] = {}
        self.__langRootBlocks[language] = None
        self.__rn = language
        self.__rl = self.__langBlocks[language]


    def block(
        self
        ,language
        ,name
    ):
        """
        Getter method. The given language name and block type name must exist in
        this factory.

        Parameters
        ----------
        language : string
                   The returned block's language name.
        name : string
               The returned block's type name.

        Returns
        -------
        ret0 : socref.Abstract.AbstractBlock
               A new block of the given type from the given language.
        """
        assert(self.__rn is None)
        return self.__langBlocks[language][name]()


    def endRegistration(
        self
    ):
        """
        Ends registration of the language currently being registered. This must
        be called only during a registration process to end it.
        """
        assert(self.__rn is not None)
        if self.__langRootBlocks[self.__rn] is None:
            del self.__langBlocks[self.__rn]
            del self.__langRootBlocks[self.__rn]
        self.__rn = None
        self.__rl = None


    def rootBlock(
        self
        ,language
    ):
        """
        Getter method. The given language must exist in this factory.

        Parameters
        ----------
        language : string
                   The returned block's language name.

        Returns
        -------
        ret0 : socref.Abstract.AbstractBlock
               A new root block from the given language.
        """
        assert(self.__rn is None)
        return self.__langRootBlocks[language]()


    def languages(
        self
    ):
        """
        Getter method.

        Returns
        -------
        ret0 : list
               Sorted language names this factory has loaded.
        """
        assert(self.__rn is None)
        ret = list(self.__langRootBlocks.keys())
        ret.sort()
        return ret


    def registerBlock(
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
        class_ : socref.Abstract.AbstractBlock
                 A class object that is registered as a block type of the
                 currently loading language.
        name : string
               The registered block's type name.
        """
        if self.__rn is None:
            raise LangError('Cannot register a block when no language is being registered.')
        if not issubclass(class_,AbstractBlock):
            raise RegisterError('Given object is not an AbstractBlock class.')
        if not name[:1].isalpha():
            raise RegisterError('Block type name must begin with alphabetical character.')
        if not name.isalnum():
            raise RegisterError('Block type name must be alphanumeric.')
        if name in self.__rl.keys():
            raise RegisterError('Block class is already registered with the same name.')
        self.__rl[name] = class_
        class_._LANG_ = self._rn
        class_._TYPE_ = name


    def registerRootBlock(
        self
        ,class_
    ):
        """
        Registers the given block class as the root block type of the language
        currently being registered. Only one class object can be the root block
        of a language.

        Parameters
        ----------
        class_ : socref.Abstract.AbstractBlock
                 A class object that is registered as the root block type of the
                 currently loading language.
        """
        if self.__rn is None:
            raise LangError('Cannot register a block when no language is being registered.')
        if not issubclass(class_,AbstractBlock):
            raise RegisterError('Given object is not an AbstractBlock class.')
        self.__langRootBlocks[self.__rn] = class_
