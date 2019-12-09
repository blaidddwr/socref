"""
Contains the C++ language parser. It also contains all helper functions and classes used for the
parsing process.
"""
import re
from socref import abstract








class Parser(abstract.AbstractParser):
    """
    This is the python parser class. It implements the Socrates' Reference abstract parser.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self, root):
        """
        Initializes a new C++ parser with the given root namespace block.

        root : A namespace block that is the root block of a C++ project that this new parser will
               parse.
        """
        abstract.AbstractParser.__init__(self)
        self.__root_block = root


    ####################
    # PUBLIC - Methods #
    ####################


    def unknown(self):
        """
        Implements the socref.abstract.AbstractParser interface.

        return : See interface docs.
        """
        pass


    #######################
    # PROTECTED - Methods #
    #######################


    def _build_(self, block, path):
        """
        Implements the socref.abstract.AbstractParser interface.

        block : See interface docs.

        path : See interface docs.

        return : See interface docs.
        """
        if path.endswith(".h"):
            return block.buildHeader({})
        elif path.endswith(".cpp"):
            return block.buildSource({})
        else:
            raise RuntimeError("Invalid path given for building.")


    def _buildPathList_(self):
        """
        Implements the socref.abstract.AbstractParser interface.
        """
        self.__buildPaths_(self.__root_block,"")


    def _scan_(self, path):
        """
        Implements the socref.abstract.AbstractParser interface.

        path : See interface docs.
        """
        self.__scan_(open(path,"r"),"")


    #####################
    # PRIVATE - Methods #
    #####################


    def __buildPaths_(self, parent, scope):
        """
        Recursively adds source code paths using the given parent block and scope.

        parent : The parent block whose children are scanned for potential paths.

        scope : The scope of the given parent block with double colons being replaced by an
                underscore.
        """
        if scope:
            scope = scope + "_"
        for child in parent:
            if child._TYPE_ == "Namespace":
                end = "" if child.isHidden() else child._p_name.lower()
                if end:
                    self._addPath_(child,scope + child._p_name.lower() + ".h")
                    if child.hasFunctions():
                        self._addPath_(child,scope + child._p_name.lower() + ".cpp")
                self.__buildPaths_(child,scope + end)
            elif child._TYPE_ == "Class":
                self._addPath_(child,scope + child._p_name.lower() + ".h")
                if not child.hasTemplates():
                    self._addPath_(child,scope + child._p_name.lower() + ".cpp")


    def __scan_(self, ifile, scope):
        """
        Detailed description.

        ifile : Detailed description.

        scope : Detailed description.
        """
        depth = 0 if scope else 1
        if scope:
            scope += "::"
        scopePattern = re.compile('^\s*(namespace|class)\s+(\w+)\s*$')
        functionPattern = re.compile('^.*\s+([\w<>,:]+)\(.*\):?$')
        while True:
            line = ifile.readline()
            if not line:
                break
            line = line[:-1]
            if line:
                depth += line.count("{") - line.count("}")
                print(depth)
                if depth <= 0:
                    break
                match = scopePattern.match(line)
                if match:
                    self.__scan_(ifile,scope + match.group(2))
                match = functionPattern.match(line)
                if match:
                    print(scope + match.group(1))
