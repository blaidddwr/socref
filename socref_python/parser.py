"""
Contains the python language parser. It also contains all helper functions and classes used for the
parsing process.
"""
import os
import re
from socref import utility as ut
from socref import abstract








class Parser(abstract.AbstractParser):
    """
    This is the python parser class. It implements the Socrates' Reference abstract parser. When
    scanning source code it builds a definitions dictionary whose keys are the path of the source
    code file scanned and values are another dictionary for that file containing three keys. The
    first key is "headers" and contains lines of import statements at the head of the python file.
    The second key is "functions" and is a dictionary whose keys are function names and values are
    their scanned lines of code. The third key is "classes" that is a dictionary whose keys are
    class names and values are dictionaries with one key "functions" that follows the same structure
    as the second "functions" key but for the methods of the class it is contained within.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self, root):
        """
        Initializes a new python parser with the given root package block.

        root : A package block that is the root block of a python project that this new parser will
               parse.
        """
        abstract.AbstractParser.__init__(self)
        self.__rootBlock = root
        self.__docPattern = re.compile(' *"""')
        self.__importPattern = re.compile('^(from|import).*')
        self.__classPattern = re.compile('^class +([a-zA-Z_]+\w*)\([\w\._]*\):')
        self.__functionPattern = re.compile('^def +([a-zA-Z_]+\w*)\(.*\):')
        self.__commentPattern = re.compile('^( *#).*')
        self.__methodPattern = re.compile('^ *def +([a-zA-Z_]+\w*)\(.*\):')
        self.__definitions = {}


    ####################
    # PUBLIC - Methods #
    ####################


    def unknown(self):
        """
        Implements the socref.abstract.AbstractParser interface.

        return : See interface docs.
        """
        ret = {}
        for key in self.__definitions:
            definition = self.__definitions[key]
            if "header" in definition:
                ret[key + ".header"] = "\n".join(definition["header"]) + "\n"
            self.__unknownFunctions_(ret,key,definition["functions"])
            for ckey in definition["classes"]:
                class_ = definition["classes"][ckey]
                self.__unknownFunctions_(ret,key + "." + ckey,class_["functions"])
        return ret


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
        return (
            "\n".join(
                block.build(
                    self.__definitions.get(path,{"header": [], "functions": {}, "classes": {}})
                )
            )
            + "\n"
        )


    def _buildPathList_(self):
        """
        Implements the socref.abstract.AbstractParser interface.
        """
        self.__buildPaths_(self.__rootBlock,"")


    def _scan_(self, path):
        """
        Implements the socref.abstract.AbstractParser interface.

        path : See interface docs.
        """
        with open(path,"r") as ifile:
            def_ = {
                "header": self.__scanHeader_(ifile)
                ,"functions": {}
                ,"classes": {}
            }
            while True:
                line = ifile.readline()
                if not line:
                    break
                line = line[:-1]
                match = self.__classPattern.match(line)
                if match:
                    ut.uniqueInsert(def_["classes"],match.group(1),self.__scanClass_(ifile))
                match = self.__functionPattern.match(line)
                if match:
                    ut.uniqueInsert(def_["functions"],match.group(1),self.__scanFunction_(ifile))
            self.__definitions[path] = def_


    #####################
    # PRIVATE - Methods #
    #####################


    def __buildPaths_(self, parent, path):
        """
        Recursively adds source code paths to be scanned using the given parent block and path.

        parent : The parent block whose children are scanned for potential source code paths.

        path : The path of the given parent block that is appended to any child block's path added.
        """
        for block in parent:
            if block._TYPE_ == "Package":
                self._addPath_(block,os.path.join(path,block._p_name,"__init__.py"))
                self.__buildPaths_(block,os.path.join(path,block._p_name))
            elif block._TYPE_ == "Module":
                self._addPath_(block,os.path.join(path,block._p_name+".py"))


    def __scanClass_(self, ifile):
        """
        Getter method.

        ifile : The python script file whose class at the current seek position is scanned.

        return : Scanned class definition containing any found and scanned method's lines of code
                 from the class definition in the given python script file at its current seek
                 position.
        """
        self.__skipDocString_(ifile)
        ret = {"functions": {}}
        scan = Scanner(ifile)
        while True:
            line = scan.readline()
            if line is None:
                break
            match = self.__methodPattern.match(line)
            if match:
                ut.uniqueInsert(ret["functions"],match.group(1),self.__scanFunction_(ifile))
        return ret


    def __scanFunction_(self, ifile):
        """
        Getter method.

        ifile : The python script file whose function at the current seek position is scanned.

        return : Scanned function definition containing all lines of scanned code along with inline
                 comment markers from the function definition in the given python script file at its
                 current seek position.
        """
        self.__skipDocString_(ifile)
        #
        # Latch is used to read in only the first line of any continuous block of comment lines.
        #
        latch = True
        ret = []
        scan = Scanner(ifile)
        while True:
            line = scan.readline()
            if line is None:
                break
            match = self.__commentPattern.match(line)
            if match:
                if latch:
                    ret.append(match.group(1)[scan.indent():])
                    latch = False
            else:
                ret.append(line[scan.indent():])
                latch = True
        return ret


    def __scanHeader_(self, ifile):
        """
        Getter method.

        ifile : The python script file whose import lines of code are scanned.

        return : A list of import code lines scanned from the header of the given python script
                 file. It is assumed all import lines occur at the beginning of the file right after
                 any initial doc string. It is also assumed the import lines end with at least one
                 blank line.
        """
        self.__skipDocString_(ifile)
        ret = []
        last = ifile.tell()
        line = ifile.readline()
        ifile.seek(last)
        if not self.__importPattern.match(line):
            return ret
        while True:
            line = ifile.readline()
            if not line or line == '\n':
                break
            ret.append(line[:-1])
        return ret


    def __skipDocString_(self, ifile):
        """
        Seeks past any initial doc string starting at the current seek index of the given python
        script file. Blank lines also skipped and ignored.

        ifile : The python script file whose doc string, if any, is skipped.
        """
        count = 0
        while count < 2:
            last = ifile.tell()
            line = ifile.readline()
            if not line:
                break
            if self.__docPattern.match(line):
                count += 1
            elif line != '\n' and count == 0:
                ifile.seek(last)
                break


    def __unknownFunctions_(self, code, root_key, definition):
        """
        Adds unknown code fragment strings to the given dictionary using the given root key for any
        remaining function lines in the given definition of functions.

        code : A dictionary of unknown code fragments that has any unused function lines added to
               it.

        root_key : A string appended with the function name and used as the key for inserting any
                   unused function definition found.

        definition : A functions definition generated from scanning and used by builders.
        """
        for key in definition:
            function = definition[key]
            if function:
                code[root_key + "." + key] = "\n".join(function) + "\n"








class Scanner():
    """
    This is the parser scanner class. It provides an object oriented scanner for an input file given
    to it at initialization. This scans python lines of code, skipping blank lines. It keeps track
    of the block of code it is scanning by the indent, stopping once a smaller indent is
    encountered.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self, ifile):
        """
        Initializes a new scanner with the given input file.

        ifile : The input file that is scanned.
        """
        self.__ifile = ifile
        self.__indent = None


    ####################
    # PUBLIC - Methods #
    ####################


    def indent(self):
        """
        Getter method.

        return : The indent size in spaces of the code block this scanner is reading. This must be
                 called after the read line method is called at least once.
        """
        if self.__indent is None:
            raise RuntimeError("indent has not been discovered.")
        return self.__indent


    def readline(self):
        """
        Getter method.

        return : The next line of python source code read in from this scanner's input file. If the
                 end of file is reached or the end of the code block with the determined indent then
                 this returns none. The first time this is called the indent of the block to be read
                 is determined with the first line of code encountered.
        """
        while True:
            last = self.__ifile.tell()
            line = self.__ifile.readline()
            if not line:
                return None
            if line != '\n':
                line = line[:-1]
                indent = len(line) - len(line.lstrip(' '))
                if self.__indent is None:
                    self.__indent = indent
                elif indent < self.__indent:
                    self.__ifile.seek(last)
                    return None
                return line
