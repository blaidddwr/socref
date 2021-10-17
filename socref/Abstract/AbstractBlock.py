"""
Contains the AbstractBlock class.
"""
from abc import ABC
from abc import abstractmethod




class AbstractBlock(ABC):
    """
    This is the abstract block class. A blocks is a model interface capable of
    containing child blocks that represents a single element of source code.
    Blocks form a tree structure to represent a single project in the
    implemented language. A block's properties are used to save all data of that
    block. Edit definitions provide a list of edit definition classes that
    inform the core application how to layout a block's edit widget interface
    for editing its properties by the user. Blocks can iterate through and index
    its children blocks. Blocks can save or load themselves and all children
    nested underneath the saved or loaded block to XML format. Volatility
    interfaces are provided to inform the core application's project model if a
    change can effect a block's parents or children.

    Blocks represent an atomic element of a programming language such as a
    variable, function, or class. Blocks can contain any number of children and
    have only one parent. The only block that does not have a parent is the root
    block of a project. One and only one special block type must be assigned as
    the root block type for each implemented language. Blocks form tree
    relationships such as a variable block being the child of a function making
    it an argument of the function. Another example is a function being a child
    of a class making it a method of the class.

    Blocks store their properties as a dictionary. Operator overloading
    interfaces must behave in such a way that any attribute that begins with
    "_p_" is stored as a property. Any property of a block that must be saved
    and loaded must begin with the given string so it is recognized as a
    property by the core application. All property attribute objects must be
    strings themselves.

    Edit definition helper classes are provided in the Edit package of the core
    application package. All possible edit widgets are defined as a class in
    said Edit package.

    An implemented block class must have no initialization arguments.
    """


    def __init__(
        self
    ):
        super().__init__()


    @abstractmethod
    def __contains__(
        self
        ,block
    ):
        pass


    @abstractmethod
    def __delitem__(
        self
        ,index
    ):
        pass


    @abstractmethod
    def __eq__(
        self
        ,other
    ):
        pass


    @abstractmethod
    def __getattr__(
        self
        ,key
    ):
        pass


    @abstractmethod
    def __getitem__(
        self
        ,index
    ):
        pass


    @abstractmethod
    def __iter__(
        self
    ):
        pass


    @abstractmethod
    def __len__(
        self
    ):
        pass


    @abstractmethod
    def __setattr__(
        self
        ,key
        ,item
    ):
        pass


    @abstractmethod
    def append(
        self
        ,block
    ):
        """
        This interface appends the given block as a new child into this block's
        list of children. The given block cannot already have a parent.

        Parameters
        ----------
        block : AbstractBlock
                The block.
        """
        pass


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
        result : QIcon
                 The icon of this block.
        """
        pass


    @abstractmethod
    def index(
        self
    ):
        """
        This interface is a getter method. This block must have a parent.

        Returns
        -------
        result : int
                 The integer index of this block within its parent block's list
                 of children.
        """
        pass


    @abstractmethod
    def insert(
        self
        ,index
        ,block
    ):
        """
        This interface inserts the given block as a new child into this block's
        list of children at the given index. The given block cannot already have
        a parent.

        Parameters
        ----------
        index : int
                The index.
        block : AbstractBlock
                The block.
        """
        pass


    @abstractmethod
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
                 parent block or false otherwise.
        """
        pass


    @abstractmethod
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
                 children blocks or false otherwise.
        """
        pass


    @abstractmethod
    def parent(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : AbstractBlock
                 The parent block of this block or none if this block has no
                 parent.
        """
        pass


    @abstractmethod
    def pop(
        self
        ,index
    ):
        """
        This interface removes a child block of this block with the given index.

        Parameters
        ----------
        index : int
                The index.

        Returns
        -------
        result : AbstractBlock
                 Removed child block.
        """
        pass


    @abstractmethod
    def properties(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : dictionary
                 Properties of this block.
        """
        pass


    @abstractmethod
    def setDefaultProperties(
        self
    ):
        """
        This interface sets all of this block's properties to their default
        state.
        """
        pass


    @abstractmethod
    def setFromXml(
        self
        ,stream
    ):
        """
        This interface loads this block's properties and all children blocks,
        including their properties, from XML using the given Qt XML reader
        stream. This overwrites any properties and children blocks this block
        may currently contain.

        Parameters
        ----------
        stream : QXmlStreamReader
                 The Qt XML reader stream.
        """
        pass


    @abstractmethod
    def setProperties(
        self
        ,properties
    ):
        """
        This interface sets this block's properties dictionary to the given
        dictionary.

        Parameters
        ----------
        properties : dictionary
                     The dictionary.
        """
        pass


    @abstractmethod
    def toXml(
        self
        ,stream
    ):
        """
        This interface saves this block's properties and all of its children
        blocks at all depths, including their properties, to XML using the given
        Qt XML writer stream.

        Parameters
        ----------
        stream : QXmlStreamWriter
                 The Qt XML stream writer.
        """
        pass
