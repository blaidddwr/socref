"""
Contains the AbstractBlock class.
"""
from ..Private import Factory
from abc import ABC
from abc import abstractmethod
from weakref import ref as WeakReference




class AbstractBlock(ABC):
    """
    This is the abstract block class. Blocks are a model interface that
    represents a single element of source code, capable of containing child
    blocks. Blocks form a tree structure to represent a single project in the
    implemented language. A block's properties are used to save all data of that
    block. Edit definitions provide a list of edit definition classes that
    inform the core application how to layout a block's edit widget interface
    for editing its properties by the user. Blocks can iterate through and index
    its children blocks. Blocks can save or load themselves and all children
    nested underneath the saved or loaded block to XML format.

    Blocks represent an atomic element of a programming language such as a
    variable, function, or class. Blocks can contain any number of children and
    have only one parent. The only block that does not have a parent is the root
    block of a project. One and only one special block type must be assigned as
    the root block type for each implemented language. Blocks form tree
    relationships such as a variable block being the child of a function making
    it an argument of the function. Another example is a function being a child
    of a class making it a method of the class.

    Blocks store their properties as a dictionary. Operator overloading is
    implemented in such a way that any attribute that begins with "_p_" is
    stored as a property. Any property of a block that must be saved and loaded
    must begin with the given string so it is recognized as a property by the
    core application.

    Edit definition helper classes are provided in the Edit package of the core
    application package. All possible edit widgets are defined as a class in
    that package that ends with Edit.

    An implemented block class must have no initialization arguments.
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
        """
        Appends the given block as a new child into this block's list of
        children. The given block cannot already have a parent.

        Parameters
        ----------
        block : socref.Abstract.AbstractBlock
                A block that is appended as a new child to this block.
        """
        self.insert(len(self),block)


    @abstractmethod
    def buildList(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : list
                 Block types that can be children of this block.
        """
        pass


    @abstractmethod
    def clearProperties(
        self
    ):
        """
        This interface clears all of this block's properties to their null
        state.
        """
        pass


    @abstractmethod
    def displayName(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : string
                 The display name of this block.
        """
        pass


    @abstractmethod
    def displayView(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : string
                 Qt rich text that gives a detailed description of this block.
        """
        pass


    @abstractmethod
    def editDefinitions(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : list
                 Edit definition classes informing the core application how to
                 build the edit GUI for this block.
        """
        pass


    @abstractmethod
    def icon(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : PySide2.QtGui.QIcon
                 The icon of this block.
        """
        pass


    def index(
        self
    ):
        """
        Getter method. This block must have a parent.

        Returns
        -------
        result : int
                 The integer index of this block within its parent block's list
                 of children.
        """
        if self.parent() is None:
            raise RuntimeError("Cannot get index of block with no parent.")
        return self.parent().__children.index(self)


    def insert(
        self
        ,index
        ,block
    ):
        """
        Inserts the given block as a new child into this block's list of
        children at the given index. The given block cannot already have a
        parent.

        Parameters
        ----------
        index : int
                The index where the given block is inserted into this block's
                list of children.
        block : socref.Abstract.AbstractBlock
                A block that is inserted as a new child to this block.
        """
        if block.parent() is not None:
            raise RuntimeError("Block is already a child of another block.")
        self.__children.insert(index,block)
        block.__parent = WeakReference(self)


    def isVolatileAbove(
        self
    ):
        """
        This interface is a getter method. The default implementation returns
        false.

        Returns
        -------
        result : bool
                 True if a change in this block's properties can effect its
                 parent block false otherwise.
        """
        return False


    def isVolatileBelow(
        self
    ):
        """
        This interface is a getter method. The default implementation returns
        false.

        Returns
        -------
        result : bool
                 True if a change in this block's properties can effect its
                 children blocks false otherwise.
        """
        return False


    def parent(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : socref.Abstract.AbstractBlock
                 The parent block of this block or none if this block has no
                 parent.
        """
        return self.__parent if self.__parent is None else self.__parent()


    def pop(
        self
        ,index
    ):
        """
        Removes a child block of this block with the given index.

        Parameters
        ----------
        index : int
                The index of the removed and returned child block.

        Returns
        -------
        result : socref.Abstract.AbstractBlock
                 Removed child block of this block with the given index.
        """
        orphan = self.__children.pop(index)
        orphan.__parent = None
        return orphan


    def properties(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : dictionary
                 Properties of this block.
        """
        return self.__properties


    @abstractmethod
    def setDefaultProperties(
        self
    ):
        """
        This interface sets all of this block's properties to their default
        state.
        """
        pass


    def setFromXml(
        self
        ,stream
    ):
        """
        Loads this block's properties and all children blocks, including their
        properties, from XML using the given qt reader stream. This overwrites
        any properties and children blocks this block may currently contain.

        Parameters
        ----------
        stream : PySide2.QtCore.QXmlStreamReader
                 Used to load all block's properties and children.
        """
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
        """
        Sets this block's properties dictionary to the given dictionary.

        Parameters
        ----------
        properties : dictionary
                     This block's new properties dictionary.
        """
        self.__properties = properties


    def toXml(
        self
        ,stream
    ):
        """
        Saves this block's properties and all of its children blocks, including
        their properties, to XML using the given qt XML writer stream.

        Parameters
        ----------
        stream : PySide2.QtCore.QXmlStreamWriter
                 Used to save all block's properties and children.
        """
        stream.writeStartElement(self._TYPE_)
        props = self.properties()
        for key in props:
            prop = props[key]
            if prop:
                stream.writeTextElement("_" + key,prop)
        for child in self:
            child.toXml(stream)
        stream.writeEndElement()
