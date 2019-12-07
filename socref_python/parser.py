"""
Contains the python language parser. It also contains all helper functions and classes used for the
parsing process.
"""
import os
import re
from socref import abstract




def unique_insert(dict_, key, item):
    """
    Inserts the given item into the given dictionary with the given key. If the given key already
    exists in the dictionary a number is appended to the key string until a key is found that does
    not exist in the dictionary.

    dict_ : The dictionary that has an item inserted into it.

    key : The key used to insert the given item into the given dictionary. If the key already exists
          this is modified.

    item : The item that is inserted into the given dictionary.
    """
    if key in dict_:
        count = 0
        key = key + str(count)
        while key in dict_:
            count += 1
            key = key[:-1] + str(count)
    dict_[key] = item








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








class Parser(abstract.Parser):
    """
    This is the python parser class. It implements the Socrates' Reference abstract parser.
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
        abstract.Parser.__init__(self)
        self.__root_block = root
        self.__doc_pattern = re.compile(' *"""')
        self.__import_pattern = re.compile('^(from|import).*')
        self.__class_pattern = re.compile('^class +([a-zA-Z_]+\w*)\([\w\._]*\):')
        self.__function_pattern = re.compile('^def +([a-zA-Z_]+\w*)\(.*\):')
        self.__comment_pattern = re.compile('^( *#).*')
        self.__method_pattern = re.compile('^ *def +([a-zA-Z_]+\w*)\(.*\):')
        self.__definitions = {}


    ####################
    # PUBLIC - Methods #
    ####################


    def unknown(self):
        """
        Implements the socref.abstract.Parser interface.

        return : See interface docs.
        """
        ret = {}
        for key in self.__definitions:
            definition = self.__definitions[key]
            if "header" in definition:
                ret[key + ".header"] = "\n".join(definition["header"]) + "\n"
            self.__unknown_functions_(ret,key,definition["functions"])
            for ckey in definition["classes"]:
                class_ = definition["classes"][ckey]
                self.__unknown_functions_(ret,key + "." + ckey,class_["functions"])
        return ret


    #######################
    # PROTECTED - Methods #
    #######################


    def _build_path_list_(self):
        """
        Implements the socref.abstract.Parser interface.
        """
        self.__build_paths_(self.__root_block,"")


    def _scan_(self, path):
        """
        Implements the socref.abstract.Parser interface.

        path : See interface docs.
        """
        with open(path,"r") as ifile:
            def_ = {
                "header": self.__scan_header_(ifile)
                ,"functions": {}
                ,"classes": {}
            }
            while True:
                line = ifile.readline()
                if not line:
                    break
                line = line[:-1]
                match = self.__class_pattern.match(line)
                if match:
                    unique_insert(def_["classes"],match.group(1),self.__scan_class_(ifile))
                match = self.__function_pattern.match(line)
                if match:
                    unique_insert(def_["functions"],match.group(1),self.__scan_function_(ifile))
            self.__definitions[path] = def_


    def _build_(self, block, path):
        """
        Implements the socref.abstract.Parser interface.

        block : See interface docs.

        path : See interface docs.

        return : See interface docs.
        """
        return block.build(
            self.__definitions.get(path,{"header": [], "functions": {}, "classes": {}})
        )


    #####################
    # PRIVATE - Methods #
    #####################


    def __build_paths_(self, parent, path):
        """
        Recursively adds source code paths to be scanned using the given parent block and path.

        parent : The parent block whose children are scanned for potential source code paths.

        path : The path of the given parent block that is appended to any child block's path added.
        """
        for block in parent:
            if block.dir_name() is not None:
                self._add_path_(block,os.path.join(path,block.dir_name(),"__init__.py"))
                self.__build_paths_(block,os.path.join(path,block.dir_name()))
            elif block.file_name() is not None:
                self._add_path_(block,os.path.join(path,block.file_name()))


    def __scan_header_(self, ifile):
        """
        Getter method.

        ifile : The python script file whose import lines of code are scanned.

        return : A list of import code lines scanned from the header of the given python script
                 file. It is assumed all import lines occur at the beginning of the file right after
                 any initial doc string. It is also assumed the import lines end with at least one
                 blank line.
        """
        self.__skip_doc_string_(ifile)
        ret = []
        last = ifile.tell()
        line = ifile.readline()
        ifile.seek(last)
        if not self.__import_pattern.match(line):
            return ret
        while True:
            line = ifile.readline()
            if not line or line == '\n':
                break
            ret.append(line[:-1])
        return ret


    def __scan_class_(self, ifile):
        """
        Getter method.

        ifile : The python script file whose class at the current seek position is scanned.

        return : Scanned class definition containing any found and scanned method's lines of code
                 from the class definition in the given python script file at its current seek
                 position.
        """
        self.__skip_doc_string_(ifile)
        ret = {"functions": {}}
        scan = Scanner(ifile)
        while True:
            line = scan.readline()
            if line is None:
                break
            match = self.__method_pattern.match(line)
            if match:
                unique_insert(ret["functions"],match.group(1),self.__scan_function_(ifile))
        return ret


    def __scan_function_(self, ifile):
        """
        Getter method.

        ifile : The python script file whose function at the current seek position is scanned.

        return : Scanned function definition containing all lines of scanned code along with inline
                 comment markers from the function definition in the given python script file at its
                 current seek position.
        """
        self.__skip_doc_string_(ifile)
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
            match = self.__comment_pattern.match(line)
            if match:
                if latch:
                    ret.append(match.group(1)[scan.indent():])
                    latch = False
            else:
                ret.append(line[scan.indent():])
                latch = True
        return ret


    def __skip_doc_string_(self, ifile):
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
            if self.__doc_pattern.match(line):
                count += 1
            elif line != '\n' and count == 0:
                ifile.seek(last)
                break


    def __unknown_functions_(self, code, root_key, definition):
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
