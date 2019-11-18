"""
Contains all abstract and interface options used by the core application and any language
implementation to communicate with one another.
"""
import abc
import os
import importlib
from . import util




def register_block(name, root=False):
    """
    Detailed description.

    name : Detailed description.

    root : Detailed description.
    """
    def wrapper(class_):
        Factory().register_block(class_,name)
        if root: Factory().register_root_block(class_)
        return class_
    return wrapper








class LoadError(Exception):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self, *args):
        """
        Detailed description.

        *args : Detailed description.
        """
        Exception.__init__(self,*args)








class RegisterError(Exception):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self, *args):
        """
        Detailed description.

        *args : Detailed description.
        """
        Exception.__init__(self,*args)








class DuplicateError(Exception):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self, *args):
        """
        Detailed description.

        *args : Detailed description.
        """
        Exception.__init__(self,*args)








class Block(abc.ABC):
    """
    Detailed description.
    """


    #######################
    # PUBLIC - Interfaces #
    #######################


    @abc.abstractmethod
    def icon(self):
        """
        Detailed description.

        return : Yes
        """
        pass


    @abc.abstractmethod
    def display_name(self):
        """
        Detailed description.

        return : Yes
        """
        pass


    @abc.abstractmethod
    def display_view(self):
        """
        Detailed description.

        return : Yes
        """
        pass


    @abc.abstractmethod
    def build_list(self):
        """
        Detailed description.

        return : Yes
        """
        pass


    @abc.abstractmethod
    def edit_definitions(self):
        """
        Detailed description.

        return : Yes
        """
        pass


    @abc.abstractmethod
    def set_default_properties(self):
        """
        Detailed description.

        return : Yes
        """
        pass


    @abc.abstractmethod
    def clear_properties(self):
        """
        Detailed description.

        return : Yes
        """
        pass


    def dir_name(self):
        """
        Detailed description.

        return : Yes
        """
        return False


    def file_name(self):
        """
        Detailed description.

        return : Yes
        """
        return None


    def is_volatile_above(self):
        """
        Detailed description.

        return : Yes
        """
        return False


    def is_volatile_below(self):
        """
        Detailed description.

        return : Yes
        """
        return False


    def parser(self):
        """
        Detailed description.

        return : Yes
        """
        pass


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self):
        """
        Detailed description.
        """
        abc.ABC.__init__(self)
        self.__children = []
        self.__parent = None
        self.__properties = {}


    #####################
    # PUBLIC - Oprators #
    #####################


    def __eq__(self, other):
        """
        Detailed description.

        other : Detailed description.

        return : Yes
        """
        return self is other


    def __len__(self):
        """
        Detailed description.

        return : Yes
        """
        return len(self.__children)


    def __contains__(self, block):
        """
        Detailed description.

        block : Detailed description.

        return : Yes
        """
        for child in self.__children :
            if child is block : return True
        return False


    def __iter__(self):
        """
        Detailed description.

        return : Yes
        """
        return self.__children.__iter__()


    def __getitem__(self, index):
        """
        Detailed description.

        index : Detailed description.

        return : Yes
        """
        return self.__children[index]


    def __delitem__(self, index):
        """
        Detailed description.

        index : Detailed description.
        """
        del self.__children[index]


    def __getattr__(self, key):
        """
        Detailed description.

        key : Detailed description.

        return : Yes
        """
        if key.startswith("_p_") : return self.__properties[key]
        else: return object.__getattribute__(self,key)


    def __setattr__(self, key, item):
        """
        Detailed description.

        key : Detailed description.

        item : Detailed description.
        """
        if key.startswith("_p_") : self.__properties[key] = item
        else: object.__setattr__(self,key,item)


    ####################
    # PUBLIC - Methods #
    ####################


    def type_name(self):
        """
        Detailed description.

        return : Yes
        """
        return self._BLOCKNAME_


    def parent(self):
        """
        Detailed description.

        return : Yes
        """
        return self.__parent


    def index(self):
        """
        Detailed description.

        return : Yes
        """
        if self.__parent is None : raise RuntimeError("Cannot get index of block with no parent.")
        return self.__parent.__children.index(self)


    def insert(self, index, block):
        """
        Detailed description.

        index : Detailed description.

        block : Detailed description.
        """
        if block.__parent is not None :
            raise RuntimeError("Block is already a child of another block.")
        self.__children.insert(index,block)
        block.__parent = self


    def append(self, block):
        """
        Detailed description.

        block : Detailed description.
        """
        self.insert(len(self),block)


    def pop(self, index):
        """
        Detailed description.

        index : Detailed description.

        return : Yes
        """
        orphan = self.__children.pop(index)
        orphan.__parent = None
        return orphan


    def to_xml(self, stream):
        """
        Detailed description.

        stream : Detailed description.
        """
        stream.writeStartElement(self._BLOCKNAME_)
        props = self.properties()
        for key in props :
            prop = props[key]
            if prop : stream.writeTextElement("_" + key,prop)
        for child in self : child.to_xml(stream)
        stream.writeEndElement()


    def set_from_xml(self, stream):
        """
        Detailed description.

        stream : Detailed description.
        """
        self.__properties = {}
        self.clear_properties()
        self.__children = []
        while not stream.atEnd() :
            stream.readNext()
            if stream.isStartElement():
                name = stream.name();
                if name.startswith("_") :
                    key = name[1:]
                    if key in self.__properties :
                        self.properties()[key] = stream.readElementText()
                else:
                    child = Factory().create(self._LANG_,name)
                    child.set_from_xml(stream)
                    self.append(child)
            elif stream.isEndElement() and stream.name() == self._BLOCKNAME_ : break


    def properties(self):
        """
        Detailed description.

        return : Yes
        """
        return self.__properties


    def set_properties(self, props):
        """
        Detailed description.

        props : Detailed description.
        """
        self.__properties = props








class Parser(abc.ABC):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self, root_block):
        """
        Detailed description.

        root_block : Detailed description.
        """
        abc.ABC.__init__(self)
        self.__root_path = ""
        self._root_block = root_block
        self.__paths = []
        self.__blocks = []


    ####################
    # PUBLIC - Methods #
    ####################


    def set_root_path(self, path):
        """
        Detailed description.

        path : Detailed description.
        """
        if self.__root_path != "" : raise RuntimeError("Root path already set.")
        self.__root_path = path


    def parse(self):
        """
        Detailed description.
        """
        if self.__root_path == "" : raise RuntimeError("Root path is not set.")
        self._build_path_list_()
        if len(set(self.__paths)) != len(self.__paths) :
            raise DuplicateError("Duplicate file names generated for parsing. This is caused by two"
                                 " blocks generating an identical file name. Perhaps check for"
                                 " blocks with the same display name and parent block.")
        for path in self.__paths :
            if os.path.isfile(path) : self._scan_(path)
        for path,block in zip(self.__paths,self.__blocks) :
            self.__build_(block,path)


    ##########################
    # PROTECTED - Interfaces #
    ##########################


    @abc.abstractmethod
    def _build_path_list_(self):
        """
        Detailed description.
        """
        pass


    @abc.abstractmethod
    def _scan_(self, path):
        """
        Detailed description.

        path : Detailed description.
        """
        pass


    @abc.abstractmethod
    def _build_(self, block, path):
        """
        Detailed description.

        block : Detailed description.

        path : Detailed description.
        """
        pass


    #######################
    # PROTECTED - Methods #
    #######################


    def _add_path_(self, block, path):
        """
        Detailed description.

        block : Detailed description.

        path : Detailed description.
        """
        if self.__root_path == "" : raise RuntimeError("Root path is not set.")
        self.__paths.append(os.path.join(self.__root_path,path))
        self.__blocks.append(block)


    #####################
    # PRIVATE - Methods #
    #####################


    def __build_(self, block, path):
        """
        Detailed description.

        block : Detailed description.

        path : Detailed description.
        """
        if not os.path.exists(os.path.dirname(path)) : os.makedirs(os.path.dirname(path))
        if os.path.exists(path):
            old = open(path,"r").read()
            new = self._build_(block,path)
            if old != new : open(path,"w").write(new)
        else: open(path,"w").write(self._build_(block,path))








@util.Singleton
class Factory():
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self):
        """
        Detailed description.
        """
        #
        self.__ROOT = "##ROOT##"
        self.__langs = {}
        self.__importing_lang = None
        self.__importing_lang_name = None


    ####################
    # PUBLIC - Methods #
    ####################


    def load(self, lang_name, import_name):
        """
        Detailed description.

        lang_name : Detailed description.

        import_name : Detailed description.
        """
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
        """
        Detailed description.

        class_ : Detailed description.

        name : Detailed description.
        """
        if name == self.__ROOT :
            raise RegisterError("Block class cannot register with reserved name.")
        self.__register_block_(class_,name)
        class_._LANG_ = self.__importing_lang_name
        class_._BLOCKNAME_ = name


    def register_root_block(self, class_):
        """
        Detailed description.

        class_ : Detailed description.
        """
        self.__register_block_(class_,self.__ROOT)


    def langs(self):
        """
        Detailed description.

        return : Yes
        """
        return self.__langs.keys()


    def blocks(self, lang_name):
        """
        Detailed description.

        lang_name : Detailed description.

        return : Yes
        """
        return self.__langs[lang_name].keys()


    def create(self, lang_name, block_name):
        """
        Detailed description.

        lang_name : Detailed description.

        block_name : Detailed description.

        return : Yes
        """
        return self.__langs[lang_name][block_name]()


    def create_root(self, lang_name):
        """
        Detailed description.

        lang_name : Detailed description.

        return : Yes
        """
        return self.__langs[lang_name][self.__ROOT]()


    #####################
    # PRIVATE - Methods #
    #####################


    def __register_block_(self, class_, name):
        """
        Detailed description.

        class_ : Detailed description.

        name : Detailed description.
        """
        if self.__importing_lang is None :
            raise RegisterError("Cannot register block class when no language is being imported.")
        if name.startswith("_") :
            raise RegisterError("Block name cannot start with an underscore.")
        if name in self.__importing_lang.keys() :
            raise RegisterError("Block class is already registered with the same name.")
        if not issubclass(class_,Block) :
            print(class_)
            raise RegisterError("Block class is not an Abstract Block.")
        self.__importing_lang[name] = class_;
