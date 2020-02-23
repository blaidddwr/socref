"""
Contains all classes and functions related to loading languages, listing them, listing their blocks,
and creating their blocks.
"""
import importlib
from . import exception
from . import utility
from . import abstract




def register(name, root=False):
    """
    Registers the wrapped class object as a block with the given name for the language currently
    being loaded. The class object is also assigned special attributes _LANG_ and _TYPE_ that
    provide the language name and block name, respectively. This must be called when a language is
    being loaded.

    Parameters
    ----------
    name : string
           The block's type name that is being registered. This must be unique among all block names
           of any one language and only contain alphabetical and underscore characters. Underscore
           characters are replaced with spaces when displays the block type to the user.
    root : bool
           Optionally indicates the registered block is the root block of the language if set to
           true. Only one block type can be the root of a language.
    """
    def wrapper(class_):
        BlockFactory().registerBlock(class_,name)
        if root:
            BlockFactory().registerRootBlock(class_)
        return class_
    return wrapper








@utility.Singleton
class BlockFactory():
    """
    This is the singleton block factory class. It creates new blocks. It loads new languages by
    importing their module and registering their block types during the load process. It provides a
    list of languages and block types per language.

    It creates new blocks. Blocks can be created by language and type or the root type of a
    language.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new factory.
        """
        self.__ROOT = "##ROOT##"
        self.__langs = {}
        self.__importingLang = None
        self.__importingLangName = None


    ####################
    # PUBLIC - Methods #
    ####################


    def blocks(self, langName):
        """
        Getter method. The given language name must exist in this factory.

        Parameters
        ----------
        langName : string
                   The language name whose list of registered block types are returned.

        Returns
        -------
        ret0 : list
               Registered block type names for a language with the given name that this factory has
               loaded.
        """
        return self.__langs[langName].keys()


    def create(self, langName, typeName):
        """
        Getter method. The given language name and block type name must exist in this factory.

        Parameters
        ----------
        langName : string
                   The name of the language of the created block.
        typeName : string
                   The type name of the created block.

        Returns
        -------
        ret0 : socref.abstract.AbstractBlock
               A new block of the given type from the given language.
        """
        return self.__langs[langName][typeName]()


    def createRoot(self, langName):
        """
        Getter method. The given language must exist in this factory.

        Parameters
        ----------
        langName : string
                   The name of the language of the created root block.

        Returns
        -------
        ret0 : socref.abstract.AbstractBlock
               A new root block from the given language.
        """
        return self.__langs[langName][self.__ROOT]()


    def langs(self):
        """
        Getter method.

        Returns
        -------
        ret0 : list
               Sorted language names this factory has loaded.
        """
        ret = list(self.__langs.keys())
        ret.sort()
        return ret


    def load(self, langName, import_name):
        """
        Loads a new language into this factory. The given language name must be unique.

        Parameters
        ----------
        langName : string
                   The language name that is loaded.
        import_name : string
                      The module name of the language that is loaded.
        """
        if langName in self.__langs.keys():
            raise exception.LangError("Language already loaded with the same name")
        self.__langs[langName] = {}
        self.__importingLang = self.__langs[langName]
        self.__importingLangName = langName
        try:
            module = importlib.import_module(import_name)
            if self.__ROOT not in self.__importingLang:
                raise exception.LangError("Language did not register a root block.")
        except:
            del(self.__langs[langName])
            raise
        finally:
            self.__importingLang = None
            self.__importingLangName = None


    def registerBlock(self, class_, name):
        """
        Registers the given block class with the given type name to the language currently being
        loaded by this factory. The given block class is assigned special attributes _LANG_ and
        _TYPE_. A language must be currently loading by this factory. The given type name must be
        unique within its language and cannot be the reserved name "##ROOT##".

        Parameters
        ----------
        class_ : socref.abstract.AbstractBlock
                 A class object that is registered as a block type of the currently loading
                 language.
        name : string
               The type name of the block that is registered.
        """
        if name == self.__ROOT:
            raise exception.RegisterError("Block class cannot register with reserved name.")
        if not name.replace("_","a").isalpha():
            raise exception.RegisterError(
                "Block type name can only contain alphabetical and underscore characters." + name
            )
        self.__registerBlock_(class_,name)
        class_._LANG_ = self.__importingLangName
        class_._TYPE_ = name


    def registerRootBlock(self, class_):
        """
        Registers the given block class as the root block type of the language currently being
        loaded by this factory. Only one class object can be the root block of a language so this
        can only be called once per language load.

        Parameters
        ----------
        class_ : socref.abstract.AbstractBlock
                 A class object that is registered as the root block type of the currently loading
                 language.
        """
        self.__registerBlock_(class_,self.__ROOT)


    #####################
    # PRIVATE - Methods #
    #####################


    def __registerBlock_(self, class_, key):
        """
        Registers the given block class with the given key to the language currently being loaded by
        this factory.

        Parameters
        ----------
        class_ : socref.abstract.AbstractBlock
                 A class object that is registered as a block type of the currently loading
                 language.
        key : string
              The key used to add the class object to the language dictionary currently being loaded
              by this factory.
        """
        if self.__importingLang is None:
            raise exception.RegisterError(
                "Cannot register block class when no language is being imported."
            )
        if key.startswith("_"):
            raise exception.RegisterError("Block type name cannot start with an underscore.")
        if key in self.__importingLang.keys():
            raise exception.RegisterError("Block class is already registered with the same name.")
        if not issubclass(class_,abstract.AbstractBlock):
            raise exception.RegisterError("Block class is not an Abstract Block.")
        self.__importingLang[key] = class_;
