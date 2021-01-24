"""
Contains the AbstractBlock class.
"""
from ..Private.Model import Factory
from abc import ABC
from abc import abstractmethod
from weakref import ref as WeakReference




class AbstractBlock(ABC):
    """
    This is the abstract block class. Blocks are the basic interface designed
    for languages to implement themselves and communicate with the core
    application. Blocks are structured to form a tree like structure to
    represent a single project. Properties are the basic concept used to save
    all data of a specific block implementation. Interface methods are organized
    into basic, property, and parsing categories. Edit definitions provide as a
    list of dictionaries through a block's appropriate interface are used by the
    core application to provide an edit GUI for it. Iteration and indexing of
    children blocks are supported through operators and methods. Saving a block
    to XML or loading it from XML is supported.

    Blocks are the basic interface designed for languages to implement
    themselves and communicate with the core application. A block should
    represent an atomic element of a programming language, such as a variable or
    function. In turn these blocks can form tree like relationships, such as
    variables being children of a function that makes them the function's
    arguments.

    Blocks are structured to form a tree like structure to represent a single
    project. Blocks can have any number of children and only one parent. There
    is a root block for every project. A specific block type is designated as
    the root block of a language and used to create a new project.

    Properties are the basic concept used to save all data of a specific block
    implementation. Properties are stored in a block as a dictionary as key
    value pairs. Operator overloading for attributes is implemented so any
    attribute that begins with "_p_" is intercepted and treated as a property.
    In this fashion all properties must be attributes that start with "_p_".

    Interface methods are organized into basic, property, and parsing
    categories. The basic category provides basic information about the block
    that is used to display it in different views, a build list to tell the core
    application what block types can be a block's child, and "is volatile"
    checks to inform the application's model if a block can effect the view of
    blocks above or below it. The property category provides edit definitions
    that allow the core application to build a form widget for editing all of a
    block's properties, setting default properties, and clearing all properties.
    The parsing category provides a factory for abstract parser implementations
    and two helper interfaces for determining if a block is the root of a source
    code file or directory.

    Edit definitions provide as a list of dictionaries through a block's
    appropriate interface are used by the core application to provide an edit
    GUI for it. Helper functions are provided in the utility module for
    producing edit definition dictionaries. The possible definition types are
    line edit, text edit, checkbox edit, combo box edit, and hidden edit. A line
    edit provides a single line to edit a property. A text edit provides a
    multi-line editor to edit a property, along with a popup text editor and
    optional spell checking. A checkbox edit provides a true or false checkbox
    to edit a property to "1" for true or "0" for false. A combo box edit
    provides a list of provided text values to edit a property.

    Iteration and indexing of children blocks are supported through operators
    and methods. Iteration of child blocks is provided through the common python
    operators. Indexing is also supported through the common python operators.
    Other common list methods are also implemented such as insert and pop for
    manipulating a block's children. Index is also provided for determining the
    index of a block in its parent's list of children.

    Saving a block to XML or loading it from XML is supported. A qt XML stream
    writer or reader is used for saving or loading to or from XML, respectively.
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
        ret0 : list
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
        ret0 : string
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
        ret0 : string
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
        ret0 : list
               Edit definition dictionaries informing the core application how
               to build the edit GUI for this block.
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
        ret0 : PySide2.QtGui.QIcon
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
        ret0 : int
               The integer index of this block within its parent block's list of
               children.
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
        ret0 : bool
               Indicates if a change in this block's properties can effect the
               blocks above it. Above in this context is going up a block's
               parent. True if this block can effect the blocks above it or
               false otherwise.
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
        ret0 : bool
               Indicates if a change in this block's properties can effect the
               blocks below it. Below in this context is going down a block's
               children. True if this block can effect the blocks below it or
               false otherwise.
        """
        return False


    def parent(
        self
    ):
        """
        Getter method.

        Returns
        -------
        ret0 : socref.Abstract.AbstractBlock
               The parent block of this block or none if this block has no
               parent.
        """
        return self.__parent if self.__parent is None else self.__parent()


    def parser(
        self
    ):
        """
        This interface is a getter method. The default implementation returns
        none.

        Returns
        -------
        ret0 : object
               An abstract parser implementation used to parse the source code
               of a project if this is the root block, else none if this is not
               the root block.
        """
        pass


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
        ret0 : socref.Abstract.AbstractBlock
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
        ret0 : dictionary
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
        Loads this block's properties and all children block's properties from
        XML using the given qt reader stream. This overwrites any properties and
        children blocks this block may currently contain.

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
        Saves this block's properties and all of its children block's properties
        to XML using the given qt XML writer stream.

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
