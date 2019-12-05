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

    name : The block's type name that is being registered. This must be unique among all block names
           of any one language.

    root : Optional root Boolean that indicates the registered block is the root block of the
           language if set to true. Only one block type can be the root of a language.
    """
    def wrapper(class_):
        Factory().register_block(class_,name)
        if root:
            Factory().register_root_block(class_)
        return class_
    return wrapper








@utility.Singleton
class Factory():
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
        self.__importing_lang = None
        self.__importing_lang_name = None


    ####################
    # PUBLIC - Methods #
    ####################


    def load(self, lang_name, import_name):
        """
        Loads a new language into this factory. The given language name must be unique.

        lang_name : The language name that is loaded.

        import_name : The module name of the language that is loaded.
        """
        if lang_name in self.__langs.keys():
            raise exception.LangError("Language already loaded with the same name")
        self.__langs[lang_name] = {}
        self.__importing_lang = self.__langs[lang_name]
        self.__importing_lang_name = lang_name
        try:
            module = importlib.import_module(import_name)
            if self.__ROOT not in self.__importing_lang:
                raise exception.LangError("Language did not register a root block.")
        except:
            del(self.__langs[lang_name])
            raise
        finally:
            self.__importing_lang = None
            self.__importing_lang_name = None


    def register_block(self, class_, name):
        """
        Registers the given block class with the given type name to the language currently being
        loaded by this factory. The given block class is assigned special attributes _LANG_ and
        _TYPE_. A language must be currently loading by this factory. The given type name must be
        unique within its language and cannot be the reserved name "##ROOT##".

        class_ : A class object that is registered as a block type of the currently loading
                 language.

        name : The type name of the block that is registered.
        """
        if name == self.__ROOT:
            raise exception.RegisterError("Block class cannot register with reserved name.")
        self.__register_block_(class_,name)
        class_._LANG_ = self.__importing_lang_name
        class_._TYPE_ = name


    def register_root_block(self, class_):
        """
        Registers the given block class as the root block type of the language currently being
        loaded by this factory. Only one class object can be the root block of a language so this
        can only be called once per language load.

        class_ : A class object that is registered as the root block type of the currently loading
                 language.
        """
        self.__register_block_(class_,self.__ROOT)


    def langs(self):
        """
        Getter method.

        return : A sorted list of languages names this factory has loaded.
        """
        ret = list(self.__langs.keys())
        ret.sort()
        return ret


    def blocks(self, lang_name):
        """
        Getter method. The given language name must exist in this factory.

        lang_name : The language name whose list of registered block types are returned.

        return : A list of registered block type names for a language with the given name that this
                 factory has loaded.
        """
        return self.__langs[lang_name].keys()


    def create(self, lang_name, type_name):
        """
        Getter method. The given language name and block type name must exist in this factory.

        lang_name : The name of the language of the created block.

        type_name : The type name of the created block.

        return : A new block of the given type from the given language.
        """
        return self.__langs[lang_name][type_name]()


    def create_root(self, lang_name):
        """
        Getter method. The given language must exist in this factory.

        lang_name : The name of the language of the created root block.

        return : A new root block from the given language.
        """
        return self.__langs[lang_name][self.__ROOT]()


    #####################
    # PRIVATE - Methods #
    #####################


    def __register_block_(self, class_, key):
        """
        Registers the given block class with the given key to the language currently being loaded by
        this factory.

        class_ : A class object that is registered as a block type of the currently loading
                 language.

        key : The key used to add the class object to the language dictionary currently being loaded
              by this factory.
        """
        if self.__importing_lang is None:
            raise exception.RegisterError(
                "Cannot register block class when no language is being imported."
            )
        if key.startswith("_"):
            raise exception.RegisterError("Block type name cannot start with an underscore.")
        if key in self.__importing_lang.keys():
            raise exception.RegisterError("Block class is already registered with the same name.")
        if not issubclass(class_,abstract.Block):
            raise exception.RegisterError("Block class is not an Abstract Block.")
        self.__importing_lang[key] = class_;
