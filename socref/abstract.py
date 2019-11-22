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
    Registers the wrapped class object as a block with the given name for the language currently
    being loaded. This must be called when a language is being loaded.

    name : The block's type name that is being registered. This must be unique among all block names
           of any one language.

    root : Optional root Boolean that indicates the registered block is the root block of the
           language if set to true. Only one block type can be the root of a language.
    """
    #
    # Define the wrapper descriptor function that actually takes the class object.
    #
    def wrapper(class_):
        #
        # Register the given class object as a new block type. If this is the root block register it
        # as such also.
        #
        Factory().register_block(class_,name)
        if root: Factory().register_root_block(class_)
        #
        # Return the class object.
        #
        return class_
    #
    # Return the wrapper function.
    #
    return wrapper








class LoadError(Exception):
    """
    This is the load error exception. This indicates an error occurred while loading a language's
    module.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, *args):
        """
        Initialize a new load error exception with any number of given positional arguments.

        *args : Positional arguments passed on to the super exception class.
        """
        Exception.__init__(self,*args)








class RegisterError(Exception):
    """
    This is the register error exception. This indicates an error occurred attempting to register a
    block while loading a language's module.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, *args):
        """
        Initialize a new load error exception with any number of given positional arguments.

        *args : Positional arguments passed on to the super exception class.
        """
        Exception.__init__(self,*args)








class ScanError(Exception):
    """
    This is the scan error exception. This indicates an error occurred while scanning source files
    while parsing a project.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, *args):
        """
        Initialize a new load error exception with any number of given positional arguments.

        *args : Positional arguments passed on to the super exception class.
        """
        Exception.__init__(self,*args)








class Block(abc.ABC):
    """
    This is the abstract block class. Blocks are the basic interface designed for languages to
    implement themselves and communicate with the core application. Blocks are structured to form a
    tree like structure to represent a single project. Properties are the basic concept used to save
    all data of a specific block implementation. Interface methods are organized into basic,
    property, and parsing categories. Edit definitions provided as a list of dictionaries through a
    block's appropriate interface are used by the core application to provide an edit GUI for it.
    Iteration and indexing of children blocks are supported through operators and methods. Saving a
    block to XML or loading it from XML is supported.

    Blocks are the basic interface designed for languages to implement themselves and communicate
    with the core application. A block should represent an atomic element of a programming language,
    such as a variable or function. In turn these blocks can form tree like relationships, such as
    variables being children of a function that makes them the function's arguments.

    Blocks are structured to form a tree like structure to represent a single project. Blocks can
    have any number of children and only one parent. There is a root block for every project. A
    specific block type is designated as the root block of a language and used to create a new
    project.

    Properties are the basic concept used to save all data of a specific block implementation.
    Properties are stored in a block as a dictionary as key value pairs. Operator overloading for
    attributes is implemented so any attribute that begins with "_p_" is intercepted and treated as
    a property. In this fashion all properties must be attributes that start with "_p_".

    Interface methods are organized into basic, property, and parsing categories. The basic category
    provides basic information about the block that is used to display it in different views, a
    build list to tell the core application what block types can be a block's child, and is volatile
    checks to inform the application's model if a block can effect the view of blocks above or below
    it. ...

    Edit definitions provided as a list of dictionaries through a block's appropriate interface are
    used by the core application to provide an edit GUI for it.

    Iteration and indexing of children blocks are supported through operators and methods.

    Saving a block to XML or loading it from XML is supported.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Detailed description.
        """
        abc.ABC.__init__(self)
        self.__children = []
        self.__parent = None
        self.__properties = {}


    #############################
    # PUBLIC - Basic Interfaces #
    #############################


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


    ################################
    # PUBLIC - Property Interfaces #
    ################################


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


    ###############################
    # PUBLIC - Parsing Interfaces #
    ###############################


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


    def parser(self):
        """
        Detailed description.

        return : Yes
        """
        pass


    ######################
    # PUBLIC - Operators #
    ######################


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


    #######################
    # PUBLIC - Initialize #
    #######################


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
            raise ScanError("Duplicate file names generated for parsing. This is caused by two"
                            " blocks generating an identical file name. Perhaps check for blocks"
                            " with the same display name and parent block.")
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

        return : Yes
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


    #######################
    # PUBLIC - Initialize #
    #######################


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
