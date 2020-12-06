"""
Contains the AbstractParser class.
"""
import abc
import os








class AbstractParser(abc.ABC):
    """
    This is the abstract parser class. A parser is the interface implemented by
    a language to parse its source code from a project. The parsing process is
    separated into building a path list, scanning, and building.

    A parser is the interface implemented by a language to parse its source code
    from a project. When the core application parses the source code of a
    project it creates a new abstract parser from the root block of the project
    that is to be parsed. The abstract parser in turn handles parsing all source
    code files using the root block of the project. A root path is also set that
    is used as the root location for all source code directories and/or files.

    The parsing process is separated into building a path list, scanning,
    building, and returning any unknown code fragments. The building a path list
    step builds a list of paths and associated blocks where source file codes
    should exist within the root path. The scanning step reads and scans every
    source code file for each path generated from the previous step. The build
    step generates new contents for each source code file generated from the
    first step, using the associated block from the first step and any scanned
    input from the previous step. The returning any unknown code fragments step
    returns any code fragments that were scanned but never used in the build
    step. If a file does not exist for a given path it is ignored in the scan
    step. Before parsing can begin the root path must be set.
    """


    def __init__(
        self
        ):
        """
        Initializes a new abstract parser.
        """
        super().__init__()
        self.__rootPath = ""
        self.__paths = []
        self.__blocks = []
        self.__buildingPaths = False


    def parse(
        self
        ,update
        ):
        """
        Parses the source code of the project of this parser's root block,
        updating its progress with the given callback object. The root path of
        this parser must be set.

        Parameters
        ----------
        update : function
                 Used to update the progress of this scan. It takes one argument
                 that is the progress as a percentage from 1 to 99.
        """
        if self.__rootPath == "":
            raise RuntimeError("Root path is not set.")
        self.__buildingPaths = True
        self._buildPathList_()
        self.__buildingPaths = False
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


    def setRootPath(
        self
        ,path
        ):
        """
        Sets the root path of this parser. This can only be called once when
        this parser's root path is empty.

        Parameters
        ----------
        path : string
               The root path of this parser.
        """
        if self.__rootPath != "":
            raise RuntimeError("Root path already set.")
        self.__rootPath = path


    @abc.abstractmethod
    def unknown(
        self
        ):
        """
        This interface is a getter method.

        Returns
        -------
        ret0 : dictionary
               A flat dictionary of scanned code that was unknown and not used
               in any built source code.
        """
        pass


    @abc.abstractmethod
    def _build_(
        self
        ,block
        ,path
        ):
        """
        This interface is a getter method.

        Parameters
        ----------
        block : socref.abstract.AbstractBlock
                The block associated with the given source code file path.
        path : string
               The path of the source code file whose contents are returned.

        Returns
        -------
        ret0 : string
               The new contents of the source code file at the given path and
               associated block.
        """
        pass


    @abc.abstractmethod
    def _buildPathList_(
        self
        ):
        """
        This interface builds the path list.
        """
        pass


    @abc.abstractmethod
    def _scan_(
        self
        ,path
        ):
        """
        This interface scans the source code file at the given path. The given
        path exists and is a regular file.

        Parameters
        ----------
        path : string
               The path of the source code file that is scanned.
        """
        pass


    def _addPath_(
        self
        ,block
        ,path
        ):
        """
        Adds the given source code file path and associated block to the list of
        paths and associated blocks to be scanned and built. This must be called
        within the build path list interface.

        Parameters
        ----------
        block : socref.abstract.AbstractBlock
                The block associated with the source code file path.
        path : string
               The source code file path.
        """
        if self.__rootPath == "":
            raise RuntimeError("Root path is not set.")
        if not self.__buildingPaths:
            raise RuntimeError("Calling add path outside of building paths.")
        self.__paths.append(os.path.join(self.__rootPath,path))
        self.__blocks.append(block)


    def __build_(
        self
        ,block
        ,path
        ):
        """
        Builds new contents for the source code at the given file path with the
        associated file. The file is overwritten with the new contents only if
        they are different from the current contents or the file does not exist.

        Parameters
        ----------
        block : socref.abstract.AbstractBlock
                The block associated with the source code file path.
        path : string
               The path to the source code file that is built.
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
