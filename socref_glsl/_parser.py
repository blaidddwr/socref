"""
Contains the Parser class.
"""
import os
import re
from socref import abstract
from socref import edit








class Parser(abstract.AbstractParser):
    """
    This is the GLSL parser class. It implements the Socrates' Reference
    abstract parser. When scanning source code it builds a definitions
    dictionary whose keys are the paths of the source code file scanned and
    values are another dictionary for that file containing two keys. The first
    key is "header" and contains any special headers for the shader. The second
    key is "functions" and is a dictionary whose keys are function signatures
    and values are their scanned lines of code.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(
        self
        ,root
        ):
        """
        Initializes a new GLSL parser with the given root program block.

        Parameters
        ----------
        root : socref_glsl.program.Program
               The root block of a GLSL project that this new parser will parse.
        """
        abstract.AbstractParser.__init__(self)
        self.__root = root
        self.__functionPattern = re.compile('^.*\s+(\w+)(\(.*)$')
        self.__definitions = {}


    ####################
    # PUBLIC - Methods #
    ####################


    def unknown(
        self
        ):
        """
        Implements the socref.abstract.AbstractParser interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = {}
        for path in self.__definitions:
            functions = self.__definitions[path]["functions"]
            for fname in functions:
                ret[path+":"+fname] = "\n".join(functions[fname])+"\n"
        return ret


    #######################
    # PROTECTED - Methods #
    #######################


    def _build_(
        self
        ,block
        ,path
        ):
        """
        Implements the socref.abstract.AbstractParser interface.

        Parameters
        ----------
        block : object
                See interface docs.
        path : object
               See interface docs.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return (
            "\n".join(block.build(self.__definitions.get(path,{"header": [], "functions": {}})))
            +"\n"
        )


    def _buildPathList_(
        self
        ):
        """
        Implements the socref.abstract.AbstractParser interface.
        """
        self.__buildPaths_(self.__root,"")


    def _scan_(
        self
        ,path
        ):
        """
        Implements the socref.abstract.AbstractParser interface.

        Parameters
        ----------
        path : object
               See interface docs.
        """
        with open(path,"r") as ifile:
            def_ = {"header": self.__scanHeader_(ifile),"functions": {}}
            while True:
                line = ifile.readline()
                if not line:
                    break
                line = line[:-1]
                if line:
                    match = self.__functionPattern.match(line)
                    if match:
                        signature = self.__scanSignature_(ifile,match.group(1),match.group(2))
                        edit.uniqueInsert(
                            def_["functions"]
                            ,signature
                            ,self.__scanFunction_(ifile,match.group(2))
                        )
            self.__definitions[path] = def_


    #####################
    # PRIVATE - Methods #
    #####################


    def __buildPaths_(
        self
        ,parent
        ,path
        ):
        """
        Recursively adds source code paths to be scanned using the given parent
        block and path.

        Parameters
        ----------
        parent : socref.abstract.AbstractBlock
                 The parent block whose children are scanned for potential
                 source code paths.
        path : string
               The path of the given parent block that is appended to any child
               block's path added.
        """
        for block in parent:
            if block._TYPE_ == "Program":
                self.__buildPaths_(block,os.path.join(path,block._p_name))
            elif block._TYPE_ == "Shader":
                self._addPath_(block,os.path.join(path,block._p_name+block.extension()))


    def __scanFunction_(
        self
        ,ifile
        ,end
        ):
        """
        Getter method.

        Parameters
        ----------
        ifile : io.TextIOWrapper
                The input file positioned right after a function declaration
                whose scanned lines are returned.
        end : string
              The end part of function declaration, beginning with the first
              opening parenthesis after the function name.

        Returns
        -------
        ret0 : list
               Scanned code lines from the given input file, assuming it is
               positioned right after the function declaration line.
        """
        depth = 0
        lines = []
        while True:
            line = ifile.readline()
            if not line:
                return lines
            line = line[:-1]
            if line:
                depth += line.count("{")-line.count("}")
                if depth <= 0:
                    return lines
                else:
                    lines.append(line)


    def __scanHeader_(
        self
        ,ifile
        ):
        """
        Getter method.

        Parameters
        ----------
        ifile : io.TextIOWrapper
                The input file whose shader directive code lines are scanned.

        Returns
        -------
        ret0 : list
               Shader directive code lines scanned from the header of the given
               shader file.
        """
        lines = []
        while True:
            line = ifile.readline()
            if not line:
                return lines
            line = line[:-1]
            if line and line.startswith("#"):
                lines.append(line)
            else:
                return lines


    def __scanSignature_(
        self
        ,ifile
        ,name
        ,end
        ):
        """
        Getter method.

        Parameters
        ----------
        ifile : io.TextIOWrapper
                The input file positioned after the first declaration line of a
                function header whose signature is scanned and returned.
        name : string
               The function name of the returned signature.
        end : string
              The ending of the first function declaration line of the returned
              signature, used for functions that have no arguments.

        Returns
        -------
        ret0 : string
               The function signature scanned from the given input file,
               assuming it is positioned at the line after the first declaration
               line. The given function name and ending is used to generate the
               signature taken from the first declaration line.
        """
        depth = end.count("(")-end.count(")")
        args = []
        while depth>0:
            line = ifile.readline()
            if not line:
                return []
            if "(" not in line and ")" not in line:
                parts = line[:-1].split()
                if len(parts)>=2:
                    arg = "_".join(parts[:-1])
                    if arg[0]==",":
                        arg = arg[1:]
                    args.append(arg)
            depth += line.count("(")-line.count(")")
        ret = name
        if args:
            ret += ":"+":".join(args)
        return ret
