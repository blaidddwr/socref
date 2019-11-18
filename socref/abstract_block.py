"""
todo
"""
import abc
from . import block_factory as bf








class Abstract_Block(abc.ABC):


    def __init__(self, lang_name, type_name):
        abc.ABC.__init__(self)
        #
        self.__lang_name = lang_name
        #
        self.__type_name = type_name
        #
        self.__children = []
        #
        self.__parent = None
        #
        self.__properties = {}
        #:


    def __eq__(self, other):
        return self is other


    def __len__(self):
        return len(self.__children)


    def __contains__(self, block):
        for child in self.__children :
            if child is block : return True
        return False


    def __iter__(self):
        return self.__children.__iter__()


    def __getitem__(self, index):
        return self.__children[index]


    def __delitem__(self, index):
        del self.__children[index]


    def __getattr__(self, key):
        if key.startswith("_p_") : return self.__properties[key]
        else: return object.__getattribute__(self,key)


    def __setattr__(self, key, item):
        if key.startswith("_p_") : self.__properties[key] = item
        else: object.__setattr__(self,key,item)


    def dir_name(self):
        return False


    def file_name(self):
        return None


    def is_volatile_above(self):
        return False


    def is_volatile_below(self):
        return False


    def parser(self):
        pass


    @abc.abstractmethod
    def icon(self):
        pass


    @abc.abstractmethod
    def display_name(self):
        pass


    @abc.abstractmethod
    def display_view(self):
        pass


    @abc.abstractmethod
    def build_list(self):
        pass


    @abc.abstractmethod
    def edit_definitions(self):
        pass


    def properties(self):
        return self.__properties


    def set_properties(self, props):
        self.__properties = props


    @abc.abstractmethod
    def set_default_properties(self):
        pass


    @abc.abstractmethod
    def clear_properties(self):
        pass


    def type_name(self):
        return self.__type_name


    def to_xml(self, stream):
        stream.writeStartElement(self.__type_name)
        props = self.properties()
        for key in props :
            prop = props[key]
            if prop : stream.writeTextElement("_" + key,prop)
        for child in self : child.to_xml(stream)
        stream.writeEndElement()


    def set_from_xml(self, stream):
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
                    child = bf.Block_Factory().create(self.__lang_name,name)
                    child.set_from_xml(stream)
                    self.append(child)
            elif stream.isEndElement() and stream.name() == self.__type_name : break


    def parent(self):
        return self.__parent


    def index(self):
        if self.__parent is None : raise RuntimeError("Cannot get index of block with no parent.")
        return self.__parent.__children.index(self)


    def insert(self,index,block):
        if block.__parent is not None :
            raise RuntimeError("Block is already a child of another block.")
        self.__children.insert(index,block)
        block.__parent = self


    def append(self, block):
        self.insert(len(self),block)


    def pop(self, index):
        orphan = self.__children.pop(index)
        orphan.__parent = None
        return orphan

