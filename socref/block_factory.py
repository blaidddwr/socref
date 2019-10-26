"""
todo
"""
from importlib import import_module
from .singleton import singleton
from .abstract_block import Abstract_Block






def register_block(name):
    """
    todo
    """
    def wrapper(class_):
        Block_Factory().register_block(class_,name)
        return class_
    return wrapper






class LoadError(Exception):
    """
    todo
    """

    def __init__(self,*args):
        Exception.__init__(self,*args)






class RegisterError(Exception):
    """
    todo
    """

    def __init__(self,*args):
        Exception.__init__(self,*args)






@singleton
class Block_Factory():
    """
    todo
    """

    def __init__(self):
        """
        todo
        """
        self.__langs = {}
        self.__importing_lang = None

    def load(self,lang_name,import_name):
        """
        todo
        """
        if lang_name in self.__langs.keys():
            raise LoadError(f"Language already loaded with name '{lang_name}'.")
        self.__langs[lang_name] = {}
        self.__importing_lang = self.__langs[lang_name]
        try:
            module = import_module(import_name)
            if not self.__importing_lang:
                raise LoadError(f"Language '{lang_name}' did not register any blocks.")
        except:
            del(self.__langs[lang_name])
            raise
        finally:
            self.__importing_lang = None

    def register_block(self,class_,name):
        """
        todo
        """
        if self.__importing_lang is None:
            raise RegisterError("Cannot register block when no language is being imported.")
        if name in self.__importing_lang.keys():
            raise RegisterError(f"Block '{name}' is already registered.")
        if not issubclass(class_,Abstract_Block):
            print(class_)
            raise RegisterError(f"Block class being registered is not an Abstract Block.")
        self.__importing_lang[name] = class_;

    def langs(self):
        """
        todo
        """
        return self.__langs.keys()

    def blocks(self,lang_name):
        """
        todo
        """
        return self.__langs[lang_name].keys()

    def create(self,lang_name,block_name):
        """
        todo
        """
        return self.__langs[lang_name][block_name]
