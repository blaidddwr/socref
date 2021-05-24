"""
Contains the BlockBase class.
"""
from ..Abstract.AbstractBlock import *
from ..Private import Factory
from weakref import ref as WeakReference




class BlockBase(AbstractBlock):
    """
    This is the block base class. It partially implements the abstract block
    class. This base class implements all required operators defined in the
    abstract class. It implements all interfaces required for the parent child
    relationship, loading to and from XML, and basic properties. Default
    interfaces for volatility above and below are implemented that both return
    false.
    """


    def __init__(
        self
    ):
        super().__init__()
        self.__children = []
        self.__parent = None
        self.__properties = {}


    def __contains__(
        self
        ,block
    ):
        return block in self.__children


    def __delitem__(
        self
        ,index
    ):
        del self.__children[index]


    def __eq__(
        self
        ,other
    ):
        return self is other


    def __getattr__(
        self
        ,key
    ):
        if key.startswith("_p_"):
            return self.__properties[key]
        else:
            return object.__getattribute__(self,key)


    def __getitem__(
        self
        ,index
    ):
        return self.__children[index]


    def __iter__(
        self
    ):
        return self.__children.__iter__()


    def __len__(
        self
    ):
        return len(self.__children)


    def __setattr__(
        self
        ,key
        ,item
    ):
        if key.startswith("_p_"):
            self.__properties[key] = item
        else:
            object.__setattr__(self,key,item)


    def append(
        self
        ,block
    ):
        self.insert(len(self),block)


    def index(
        self
    ):
        if self.parent() is None:
            raise RuntimeError("Cannot get index of block with no parent.")
        return self.parent().__children.index(self)


    def insert(
        self
        ,index
        ,block
    ):
        if block.parent() is not None:
            raise RuntimeError("Block is already a child of another block.")
        self.__children.insert(index,block)
        block.__parent = WeakReference(self)


    def isVolatileAbove(
        self
    ):
        return False


    def isVolatileBelow(
        self
    ):
        return False


    def parent(
        self
    ):
        return self.__parent if self.__parent is None else self.__parent()


    def pop(
        self
        ,index
    ):
        orphan = self.__children.pop(index)
        orphan.__parent = None
        return orphan


    def properties(
        self
    ):
        return self.__properties


    def setFromXml(
        self
        ,stream
    ):
        self.__properties = {}
        self.clearProperties()
        self.__children = []
        endName = self._TYPE_.replace(" ","_")
        while not stream.atEnd():
            stream.readNext()
            if stream.isStartElement():
                name = stream.name()
                if name.startswith("_"):
                    key = name[1:]
                    if key in self.__properties:
                        self.__properties[key] = stream.readElementText()
                else:
                    child = Factory.blockFactory.create(self._LANG_,name)
                    child.setFromXml(stream)
                    self.append(child)
            elif stream.isEndElement() and stream.name() == endName:
                break


    def setProperties(
        self
        ,properties
    ):
        self.__properties = properties


    def toXml(
        self
        ,stream
    ):
        stream.writeStartElement(self._TYPE_)
        props = self.properties()
        for key in props:
            prop = props[key]
            if prop:
                stream.writeTextElement("_" + key,prop)
        for child in self:
            child.toXml(stream)
        stream.writeEndElement()
