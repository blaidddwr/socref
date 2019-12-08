"""
Contains all abstract classes used by the core application and any language implementation to
communicate with one another.
"""
import abc
import weakref
import os
from . import exception
from . import block








class AbstractBlock(abc.ABC):
    """
    This is the abstract block class. Blocks are the basic interface designed for languages to
    implement themselves and communicate with the core application. Blocks are structured to form a
    tree like structure to represent a single project. Properties are the basic concept used to save
    all data of a specific block implementation. Interface methods are organized into basic,
    property, and parsing categories. Edit definitions provide as a list of dictionaries through a
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
    build list to tell the core application what block types can be a block's child, and "is
    volatile" checks to inform the application's model if a block can effect the view of blocks
    above or below it. The property category provides edit definitions that allow the core
    application to build a form widget for editing all of a block's properties, setting default
    properties, and clearing all properties. The parsing category provides a factory for abstract
    parser implementations and two helper interfaces for determining if a block is the root of a
    source code file or directory.

    Edit definitions provide as a list of dictionaries through a block's appropriate interface are
    used by the core application to provide an edit GUI for it. Helper functions are provided in the
    utility module for producing edit definition dictionaries. The possible definition types are
    line edit, text edit, checkbox edit, combo box edit, and hidden edit. A line edit provides a
    single line to edit a property. A text edit provides a multi-line editor to edit a property,
    along with a popup text editor and optional spell checking. A checkbox edit provides a true or
    false checkbox to edit a property to "1" for true or "0" for false. A combo box edit provides a
    list of provided text values to edit a property.

    Iteration and indexing of children blocks are supported through operators and methods. Iteration
    of child blocks is provided through the common python operators. Indexing is also supported
    through the common python operators. Other common list methods are also implemented such as
    insert and pop for manipulating a block's children. Index is also provided for determining the
    index of a block in its parent's list of children.

    Saving a block to XML or loading it from XML is supported. A qt XML stream writer or reader is
    used for saving or loading to or from XML, respectively.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new abstract block.
        """
        abc.ABC.__init__(self)
        self.__children = []
        self.__parent = None
        self.__properties = {}


    #####################
    # PUBLIC - Abstract #
    #####################


    @abc.abstractmethod
    def icon(self):
        """
        This interface is a getter method.

        return : A qt icon that represents this block.
        """
        pass


    @abc.abstractmethod
    def display_name(self):
        """
        This interface is a getter method.

        return : A string that is the display name of this block.
        """
        pass


    @abc.abstractmethod
    def display_view(self):
        """
        This interface is a getter method.

        return : A string in qt rich text format that gives a detailed description of this block.
        """
        pass


    @abc.abstractmethod
    def build_list(self):
        """
        This interface is a getter method.

        return : A list of block types that can be children of this block.
        """
        pass


    @abc.abstractmethod
    def edit_definitions(self):
        """
        This interface is a getter method.

        return : A list of edit definition dictionaries informing the core application how to build
                 the edit GUI for this block.
        """
        pass


    @abc.abstractmethod
    def set_default_properties(self):
        """
        This interface sets all of this block's properties to their default state.
        """
        pass


    @abc.abstractmethod
    def clear_properties(self):
        """
        This interface clears all of this block's properties to their null state.
        """
        pass


    ######################
    # PUBLIC - Operators #
    ######################


    def __eq__(self, other):
        """
        Implements the equality operator.

        other : Another block that is evaluated for equality with this one.

        return : True if the other object is this instance or false otherwise.
        """
        return self is other


    def __len__(self):
        """
        Implements the length operator.

        return : Total number of children of this block.
        """
        return len(self.__children)


    def __contains__(self, block):
        """
        Implements the contain operator.

        block : Another block that is evaluated.

        return : True if the given block is a child of this block or false otherwise.
        """
        return block in self.__children


    def __iter__(self):
        """
        Implements the iterator operator.

        return : Iterator of this block's list of children.
        """
        return self.__children.__iter__()


    def __getitem__(self, index):
        """
        Implements the get item operator.

        index : The integer index of this block's child block that is returned.

        return : This block's child block with the given index.
        """
        return self.__children[index]


    def __delitem__(self, index):
        """
        Implements the delete item operator. Deletes this block's child block with the given integer
        index.

        index : The integer index of this block's child block that is deleted.
        """
        del self.__children[index]


    def __getattr__(self, key):
        """
        Implements the get attribute operator. Intercepts the value of any property attribute.

        key : The key of the attribute whose value is returned.

        return : The property attribute that begins with "_p_" or the default get attribute operator
                 return value otherwise.
        """
        if key.startswith("_p_"):
            return self.__properties[key]
        else:
            return object.__getattribute__(self,key)


    def __setattr__(self, key, item):
        """
        Implements the set attribute operator. Intercepts and sets the value of any property
        attribute that begins with "_p_" to the given item, else calls the default set attribute
        operator to the given item.

        key : The key of the attribute whose value is set to the given item.

        item : An item used to set the attribute with the given key.
        """
        if key.startswith("_p_"):
            self.__properties[key] = item
        else:
            object.__setattr__(self,key,item)


    ####################
    # PUBLIC - Methods #
    ####################


    def parser(self):
        """
        This interface is a getter method. The default implementation returns none.

        return : An abstract parser implementation used to parse the source code of a project if
                 this is the root block, else none if this is not the root block.
        """
        pass


    def is_volatile_below(self):
        """
        This interface is a getter method. The default implementation returns false.

        return : A Boolean indicating if a change in this block's properties can effect the blocks
                 below it. Below in this context is going down a block's children. True if this
                 block can effect the blocks below it or false otherwise.
        """
        return False


    def is_volatile_above(self):
        """
        This interface is a getter method. The default implementation returns false.

        return : A Boolean indicating if a change in this block's properties can effect the blocks
                 above it. Above in this context is going up a block's parent. True if this block
                 can effect the blocks above it or false otherwise.
        """
        return False


    ####################
    # PUBLIC - Methods #
    ####################


    def parent(self):
        """
        Getter method.

        return : The parent block of this block or none if this block has no parent.
        """
        return self.__parent if self.__parent is None else self.__parent()


    def index(self):
        """
        Getter method. This block must have a parent.

        return : The integer index of this block within its parent block's list of children.
        """
        if self.parent() is None:
            raise RuntimeError("Cannot get index of block with no parent.")
        return self.parent().__children.index(self)


    def insert(self, index, block):
        """
        Inserts the given block as a new child into this block's list of children at the given
        index. The given block cannot already have a parent.

        index : The index where the given block is inserted into this block's list of children.

        block : A block that is inserted as a new child to this block.
        """
        if block.parent() is not None:
            raise RuntimeError("Block is already a child of another block.")
        self.__children.insert(index,block)
        block.__parent = weakref.ref(self)


    def append(self, block):
        """
        Appends the given block as a new child into this block's list of children. The given block
        cannot already have a parent.

        block : A block that is appended as a new child to this block.
        """
        self.insert(len(self),block)


    def pop(self, index):
        """
        Removes a child block of this block with the given index.

        index : The index of the removed and returned child block.

        return : Removed child block of this block with the given index.
        """
        orphan = self.__children.pop(index)
        orphan.__parent = None
        return orphan


    def to_xml(self, stream):
        """
        Saves this block's properties and all of its children block's properties to XML using the
        given qt XML writer stream.

        stream : A qt writer stream used to save all block's properties and children.
        """
        stream.writeStartElement(self._TYPE_)
        props = self.properties()
        for key in props:
            prop = props[key]
            if prop:
                stream.writeTextElement("_" + key,prop)
        for child in self:
            child.to_xml(stream)
        stream.writeEndElement()


    def set_from_xml(self, stream):
        """
        Loads this block's properties and all children block's properties from XML using the given
        qt reader stream. This overwrites any properties and children blocks this block may
        currently contain.

        stream : A qt reader stream used to load all block's properties and children.
        """
        self.__properties = {}
        self.clear_properties()
        self.__children = []
        while not stream.atEnd():
            stream.readNext()
            if stream.isStartElement():
                name = stream.name()
                if name.startswith("_"):
                    key = name[1:]
                    if key in self.__properties:
                        self.__properties[key] = stream.readElementText()
                else:
                    child = block.BlockFactory().create(self._LANG_,name)
                    child.set_from_xml(stream)
                    self.append(child)
            elif stream.isEndElement() and stream.name() == self._TYPE_:
                break


    def properties(self):
        """
        Getter method.

        return : Properties dictionary of this block.
        """
        return self.__properties


    def set_properties(self, props):
        """
        Sets this block's properties dictionary to the given dictionary.

        props : A dictionary that is set as this block's new properties dictionary.
        """
        self.__properties = props








class AbstractParser(abc.ABC):
    """
    This is the abstract parser class. A parser is the interface implemented by a language to parse
    its source code from a project. The parsing process is separated into building a path list,
    scanning, and building.

    A parser is the interface implemented by a language to parse its source code from a project.
    When the core application parses the source code of a project it creates a new abstract parser
    from the root block of the project that is to be parsed. The abstract parser in turn handles
    parsing all source code files using the root block of the project. A root path is also set that
    is used as the root location for all source code directories and/or files.

    The parsing process is separated into building a path list, scanning, building, and returning
    any unknown code fragments. The building a path list step builds a list of paths and associated
    blocks where source file codes should exist within the root path. The scanning step reads and
    scans every source code file for each path generated from the previous step. The build step
    generates new contents for each source code file generated from the first step, using the
    associated block from the first step and any scanned input from the previous step. The returning
    any unknown code fragments step returns any code fragments that were scanned but never used in
    the build step. If a file does not exist for a given path it is ignored in the scan step. Before
    parsing can begin the root path must be set.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new abstract parser.
        """
        abc.ABC.__init__(self)
        self.__root_path = ""
        self.__paths = []
        self.__blocks = []
        self.__building_paths = False


    #######################
    # PUBLIC - Interfaces #
    #######################


    @abc.abstractmethod
    def unknown(self):
        """
        This interface is a getter method.

        return : A flat dictionary of scanned code that was unknown and not used in any built source
                 code.
        """
        pass


    ####################
    # PUBLIC - Methods #
    ####################


    def set_root_path(self, path):
        """
        Sets the root path of this parser. This can only be called once when this parser's root path
        is empty.

        path : The root path of this parser.
        """
        if self.__root_path != "":
            raise RuntimeError("Root path already set.")
        self.__root_path = path


    def parse(self, update):
        """
        Parses the source code of the project of this parser's root block, updating its progress
        with the given callback object. The root path of this parser must be set.

        update : A callable object that is used to update the progress of this scan. It takes one
                 argument that is the progress as a percentage from 1 to 99.
        """
        if self.__root_path == "":
            raise RuntimeError("Root path is not set.")
        self.__building_paths = True
        self._build_path_list_()
        self.__building_paths = False
        #
        # Make sure there are no duplicates paths in the generated path list.
        #
        if len(set(self.__paths)) != len(self.__paths):
            raise exception.ScanError("Duplicate file names generated for parsing. This is caused"
                                      " by two blocks generating an identical file name. Perhaps"
                                      " check for blocks with the same display name and parent"
                                      " block.")
        count = 0
        for path in self.__paths:
            if os.path.isfile(path):
                self._scan_(path)
            count += 1
            update(count * 50 // len(self.__paths))
        count = 0
        for path,block in zip(self.__paths,self.__blocks):
            self.__build_(block,path)
            count += 1
            update(50 + (count * 50 // len(self.__paths)))
        self.__paths = []
        self.__blocks = []


    ##########################
    # PROTECTED - Interfaces #
    ##########################


    @abc.abstractmethod
    def _build_path_list_(self):
        """
        This interface builds the path list.
        """
        pass


    @abc.abstractmethod
    def _scan_(self, path):
        """
        This interface scans the source code file at the given path. The given path exists and is a
        regular file.

        path : The path of the source code file that is scanned.
        """
        pass


    @abc.abstractmethod
    def _build_(self, block, path):
        """
        This interface is a getter method.

        block : The block associated with the given source code file path.

        path : The path of the source code file whose contents are returned.

        return : The new string contents of the source code file at the given path and associated
                 block.
        """
        pass


    #######################
    # PROTECTED - Methods #
    #######################


    def _add_path_(self, block, path):
        """
        Adds the given source code file path and associated block to the list of paths and
        associated blocks to be scanned and built. This must be called within the build path list
        interface.

        block : The block associated with the source code file path.

        path : The source code file path.
        """
        if self.__root_path == "":
            raise RuntimeError("Root path is not set.")
        if not self.__building_paths:
            raise RuntimeError("Calling add path outside of building paths.")
        self.__paths.append(os.path.join(self.__root_path,path))
        self.__blocks.append(block)


    #####################
    # PRIVATE - Methods #
    #####################


    def __build_(self, block, path):
        """
        Builds new contents for the source code at the given file path with the associated file. The
        file is overwritten with the new contents only if they are different from the current
        contents or the file does not exist.

        block : The block associated with the source code file path.

        path : The path to the source code file that is built.
        """
        if not os.path.exists(os.path.dirname(path)):
            os.makedirs(os.path.dirname(path))
        if os.path.exists(path):
            old = open(path,"r").read()
            new = self._build_(block,path)
            if old != new:
                open(path,"w").write(new)
        else:
            open(path,"w").write(self._build_(block,path))
