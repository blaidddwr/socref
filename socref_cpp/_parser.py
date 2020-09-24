"""
Contains the Parser class.
"""
import re
from socref import abstract
from socref import edit
from . import settings








class Parser(abstract.AbstractParser):
    """
    This is the python parser class. It implements the Socrates' Reference
    abstract parser. When scanning source code it builds a definitions
    dictionary that contains two keys. The first key is "headers" and contains
    another dictionary whose keys are scanned paths and values are scanned
    preprocessor header lines. The second is "functions" and contains another
    dictionary whose keys are function signatures and values are scanned
    function definition code lines.
    """


    def __init__(
        self
        ,root
        ):
        """
        Initializes a new C++ parser with the given root namespace block.

        Parameters
        ----------
        root : socref_cpp.namespace.Namespace
               The root block of a C++ project that this new parser will parse.
        """
        super().__init__()
        self.__root_block = root
        self.__scopePattern = re.compile('^\s*namespace\s+(\w+)\s*$')
        self.__functionPattern = re.compile('^(.*\s)?([\S]+)(\(.*)$')
        self.__functionPointerPattern = re.compile('^(.*)\(\*\w+\)\((.*)\)')
        self.__definitions = {"headers": {},"functions": {}}


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
        return {
            key: "\n".join(self.__definitions["functions"][key])
            for key in self.__definitions["functions"]
        }


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
        if path.endswith(settings.HEADER_EXTENSION):
            return "\n".join(block.buildHeader(self.__definitions,path))+"\n"
        elif path.endswith(settings.SOURCE_EXTENSION):
            return "\n".join(block.buildSource(self.__definitions,path))+"\n"
        else:
            raise RuntimeError("Invalid path given for building.")


    def _buildPathList_(
        self
        ):
        """
        Implements the socref.abstract.AbstractParser interface.
        """
        self.__buildPaths_(self.__root_block,"")


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
            headers = self.__scanHeader_(ifile)
            if path.endswith(settings.HEADER_EXTENSION):
                headers = headers[2:]
            self.__definitions["headers"][path] = headers
            self.__scan_(ifile,"")


    def __buildPaths_(
        self
        ,parent
        ,scope
        ):
        """
        Recursively adds source code paths using the given parent block and
        scope.

        Parameters
        ----------
        parent : socref.abstract.AbstractBlock
                 The parent block whose children are scanned for potential
                 paths.
        scope : string
                The scope of the given parent block with double colons being
                replaced by an underscore.
        """
        if scope:
            scope = scope + "_"
        for child in parent:
            if child._TYPE_ == "Namespace":
                end = "" if child.isHidden() else child._p_name.lower()
                self._addPath_(child,scope+child._p_name.lower()+settings.HEADER_EXTENSION)
                if child.hasFunctions():
                    self._addPath_(child,scope+child._p_name.lower()+settings.SOURCE_EXTENSION)
                self.__buildPaths_(child,scope + end)
            elif child._TYPE_ == "Class":
                self._addPath_(child,scope+child._p_name.lower()+settings.HEADER_EXTENSION)
                if not child.hasTemplates():
                    self._addPath_(child,scope+child._p_name.lower()+settings.SOURCE_EXTENSION)
            elif not scope and child._TYPE_ == "Function" and child._p_name == "main":
                self._addPath_(child,child._p_name.lower()+settings.SOURCE_EXTENSION)


    def __scan_(
        self
        ,ifile
        ,scope
        ):
        """
        Recursively scans the given input file for code of function definitions
        enclosed in the given namespace scope. Any scanned code of function
        definitions are added to this parser's definitions dictionary.

        Parameters
        ----------
        ifile : io.TextIOWrapper
                The input file that is scanned for function definition code.
        scope : string
                The namespace scope that any found function definitions are
                nested within.
        """
        depth = 0 if scope else 1
        if scope:
            scope += "::"
        while True:
            line = ifile.readline()
            if not line:
                break
            line = line[:-1]
            if line:
                depth += line.count("{") - line.count("}")
                if depth <= 0:
                    break
                match = self.__scopePattern.match(line)
                if match:
                    self.__scan_(ifile,scope + match.group(1))
                if not line.startswith("template"):
                    match = self.__functionPattern.match(line)
                    if match:
                        signature = self.__scanSignature_(ifile,match.group(2),match.group(3))
                        if signature:
                            edit.uniqueInsert(
                                self.__definitions["functions"]
                                ,scope+signature
                                ,self.__scanFunction_(ifile)
                            )


    def __scanArgument_(
        self
        ,line
        ):
        """
        Getter method.

        Parameters
        ----------
        line : string
               A function argument whose signature is returned.

        Returns
        -------
        ret0 : string
               The argument signature of the given argument line. The signature
               removes any spaces and the name of the argument, leaving only the
               type. This only works with arguments where the last word is the
               variable name separated from its type, including asterisks or
               ampersands. This also works with C style function pointers.
        """
        line = line.lstrip()
        if line.startswith(","):
            line = line[1:]
        match = self.__functionPointerPattern.match(line)
        if match:
            return "%s(*)(%s)"%(match.group(1).replace(" ",""),match.group(2).replace(" ",""))
        else:
            subs = line.split()
            return "".join(subs[:-1])


    def __scanFunction_(
        self
        ,ifile
        ):
        """
        Getter method.

        Parameters
        ----------
        ifile : io.TextIOWrapper
                The input file positioned at the beginning of a function
                definition whose lines of code are scanned and returned.

        Returns
        -------
        ret0 : list
               Code lines scanned from the given input file, assuming it is at
               the beginning of a function definition. It stops scanning lines
               when the final enclosing bracket is found. The line with the last
               ending bracket is not included with the returned lines.
        """
        depth = 1
        lines = []
        first = False
        while True:
            line = ifile.readline()
            if not line:
                return lines
            line = line[:-1]
            if line:
                if not first and "{" in line:
                    depth -= 1
                    first = True
                depth += line.count("{")-line.count("}")
                if depth <= 0:
                    return lines
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
                The input file positioned at the beginning of the file whose
                preprocessor header lines are scanned and returned.

        Returns
        -------
        ret0 : list
               Preprocessor header lines scanned from the given input file,
               assuming it is at the beginning of the file. This will include
               the header guard lines if the given input file is a C++ header.
        """
        lines = []
        while True:
            last = ifile.tell()
            line = ifile.readline()
            if not line:
                return lines
            line = line[:-1]
            if not line:
                return lines
            else:
                lines.append(line)


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
               signature taken from the first declaration line. The returned
               signature does not include any scope.
        """
        depth = end.count("(")-end.count(")")
        args = []
        line = end
        while depth>0:
            line = ifile.readline()
            if not line:
                return None
            if "(" not in line and ")" not in line:
                parts = line[:-1].split()
                if len(parts)>=2:
                    arg = "_".join(parts[:-1])
                    if arg[0]==",":
                        arg = arg[1:]
                    args.append(arg)
            depth += line.count("(")-line.count(")")
        if ";" in line:
            return None
        if "const" in line:
            args.append("const")
        ret = name
        if args:
            ret += ":"+":".join(args)
        return ret
