from importlib import import_module
from .singleton import singleton
from .abstract_block import Abstract_Block






def register_block(name,root=False):
    #.
    def wrapper(class_):
        Block_Factory().register_block(class_,name)
        if root: Block_Factory().register_root_block(class_)
        return class_
    #.
    return wrapper






class LoadError(Exception):


    def __init__(self,*args):
        Exception.__init__(self,*args)






class RegisterError(Exception):


    def __init__(self,*args):
        Exception.__init__(self,*args)






@singleton
class Block_Factory():


    def __init__(self):
        # 
        self.ROOT = "__ROOT__"
        # 
        self.__langs = {}
        # 
        self.__importing_lang = None
        #:


    def load(self,lang_name,import_name):
        #.
        if lang_name in self.__langs.keys():
            raise LoadError("Language already loaded with the same name")
        #.
        self.__langs[lang_name] = {}
        self.__importing_lang = self.__langs[lang_name]
        try:
            #.
            module = import_module(import_name)
            if not self.__importing_lang:
                raise LoadError("Language did not register any blocks.")
        #.
        except:
            del(self.__langs[lang_name])
            raise
        #.
        finally:
            self.__importing_lang = None


    def register_block(self,class_,name):
        #.
        if name == self.ROOT:
            raise RegisterError(f"Block class cannot register with reserved name {self.ROOT}.")
        #.
        self.__register_block(class_,name)


    def register_root_block(self,class_):
        self.__register_block(class_,self.ROOT)


    def langs(self):
        return self.__langs.keys()


    def blocks(self,lang_name):
        return self.__langs[lang_name].keys()


    def create(self,lang_name,block_name):
        return self.__langs[lang_name][block_name]


    def create_root(self,lang_name):
        return self.__langs[lang_name][self.ROOT]


    def __register_block(self,class_,name):
        #.
        if self.__importing_lang is None:
            raise RegisterError("Cannot register block class when no language is being imported.")
        if name in self.__importing_lang.keys():
            raise RegisterError("Block class is already registered with the same name.")
        if not issubclass(class_,Abstract_Block):
            print(class_)
            raise RegisterError("Block class is not an Abstract Block.")
        #.
        self.__importing_lang[name] = class_;
