"""
todo
"""
import importlib
from . import singleton as si
from . import abstract_block as ab






def register_block(name, root=False):
    def wrapper(class_):
        Block_Factory().register_block(class_,name)
        if root: Block_Factory().register_root_block(class_)
        return class_
    return wrapper






class LoadError(Exception):


    def __init__(self,*args):
        Exception.__init__(self,*args)






class RegisterError(Exception):


    def __init__(self,*args):
        Exception.__init__(self,*args)






@si.Singleton
class Block_Factory():


    def __init__(self):
        #
        self.__ROOT = "##ROOT##"
        #
        self.__langs = {}
        #
        self.__importing_lang = None
        #
        self.__importing_lang_name = None
        #:


    def load(self, lang_name, import_name):
        if lang_name in self.__langs.keys() :
            raise LoadError("Language already loaded with the same name")
        self.__langs[lang_name] = {}
        self.__importing_lang = self.__langs[lang_name]
        self.__importing_lang_name = lang_name
        try:
            module = importlib.import_module(import_name)
            if not self.__importing_lang:
                raise LoadError("Language did not register any blocks.")
        except:
            del(self.__langs[lang_name])
            raise
        finally:
            self.__importing_lang = None
            self.__importing_lang_name = None


    def register_block(self, class_, name):
        if name == self.__ROOT :
            raise RegisterError("Block class cannot register with reserved name.")
        self.__register_block_(class_,name)
        class_._LANG_ = self.__importing_lang_name
        class_._BLOCKNAME_ = name


    def register_root_block(self, class_):
        self.__register_block_(class_,self.__ROOT)


    def langs(self):
        return self.__langs.keys()


    def blocks(self, lang_name):
        return self.__langs[lang_name].keys()


    def create(self, lang_name, block_name):
        return self.__langs[lang_name][block_name]()


    def create_root(self, lang_name):
        return self.__langs[lang_name][self.__ROOT]()


    def __register_block_(self, class_, name):
        if self.__importing_lang is None :
            raise RegisterError("Cannot register block class when no language is being imported.")
        if name.startswith("_") :
            raise RegisterError("Block name cannot start with an underscore.")
        if name in self.__importing_lang.keys() :
            raise RegisterError("Block class is already registered with the same name.")
        if not issubclass(class_,ab.Abstract_Block) :
            print(class_)
            raise RegisterError("Block class is not an Abstract Block.")
        self.__importing_lang[name] = class_;
