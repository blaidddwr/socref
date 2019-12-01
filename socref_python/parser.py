"""
Contains the python language parser. It also contains all helper functions and classes used for the
parsing process.
"""
import os
import re
from socref import abstract




def uniq_insert(dict_, key, item):
    """
    Inserts the given item into the given dictionary with the given key. If the given key already
    exists in the dictionary a number is appended to the key string until a key is found that does
    not exist in the dictionary.

    dict_ : The dictionary that has an item inserted into it.

    key : The key used to insert the given item into the given dictionary. If the key already exists
          this is modified.

    item : The item that is inserted into the given dictionary.
    """
    #
    # Check to see if the given key already exists in the given dictionary.
    #
    if key in dict_ :
        #
        # Keep appending a number to the key, starting at 0, until an unused key is found.
        #
        count = 0
        key = key + str(count)
        while key in dict_ :
            count += 1
            key = key[:-1] + str(count)
    #
    # Insert the given item into the given dictionary with the unused key.
    #
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
        if self.__indent is None :
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
        #
        # Continue until a line of code is found or an end condition is meant.
        #
        while True :
            #
            # Get the seek location of the file and then read in the next line.
            #
            last = self.__ifile.tell()
            line = self.__ifile.readline()
            #
            # If the end of file has been reached then return none.
            #
            if not line : return None
            #
            # Check to see if this is not a blank line.
            #
            if line != '\n' :
                #
                # Remove the new line character and determine the line's indent.
                #
                line = line[:-1]
                indent = len(line) - len(line.lstrip(' '))
                #
                # If the indent has not yet been set then set it with this line's indent.
                #
                if self.__indent is None : self.__indent = indent
                #
                # Else make sure this line's indent is within the block being read.
                #
                elif indent < self.__indent :
                    #
                    # Revert the seek location back to the previous because the read in line is not
                    # part of block being scanned, and then return none.
                    #
                    self.__ifile.seek(last)
                    return None
                #
                # Return the read in line that is part of the code block being scanned.
                #
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
        abstract.Parser.__init__(self,root)
        #
        # Initialize all compiled regular expressions used by this parser.
        #
        self.__doc_pattern = re.compile(' *"""')
        self.__import_pattern = re.compile('^(from|import).*')
        self.__class_pattern = re.compile('^class +([a-zA-Z_]+\w*)\([\w\._]*\):')
        self.__function_pattern = re.compile('^def +([a-zA-Z_]+\w*)\(.*\):')
        self.__comment_pattern = re.compile('^( *#).*')
        self.__method_pattern = re.compile('^ *def +([a-zA-Z_]+\w*)\(.*\):')
        #
        # Initialize the definitions dictionary used to save all scanned lines of code for this
        # parser.
        #
        self.__defs = {}


    #######################
    # PROTECTED - Methods #
    #######################


    def _build_path_list_(self):
        """
        Implements the socref.abstract.Parser interface.
        """
        self.__build_paths_(self._root_block_(),"")


    def _scan_(self, path):
        """
        Implements the socref.abstract.Parser interface.

        path : See interface docs.
        """
        #
        # Open the file at the given path as read only.
        #
        with open(path,"r") as ifile :
            #
            # Initialize a path scanning definition.
            #
            def_ = {"header": self.__scan_header_(ifile)
                    ,"functions": {}
                    ,"classes": {}}
            #
            # Continue while there are still lines to read.
            #
            while True :
                #
                # Read the next line, breaking if it is the end of the file.
                #
                line = ifile.readline()
                if not line : break
                #
                # Remove the newline character from the end.
                #
                line = line[:-1]
                #
                # Match the line to see if it is a class or function header, scanning its source
                # code and inserting the returned class or function definition into the path
                # definition. Use a unique key for insertion so it does not overwrite anything else
                # scanned.
                #
                match = self.__class_pattern.match(line)
                if match : uniq_insert(def_["classes"],match.group(1),self.__scan_class_(ifile))
                match = self.__function_pattern.match(line)
                if match :
                    uniq_insert(def_["functions"],match.group(1),self.__scan_function_(ifile))
            #
            # Insert the scanned path definition into this parser's dictionary of scanned paths.
            #
            self.__defs[path] = def_


    def _build_(self, block, path):
        """
        Implements the socref.abstract.Parser interface.

        block : See interface docs.

        path : See interface docs.

        return : See interface docs.
        """
        return block.build(self.__defs.get(path,{"header": [], "functions": {}, "classes": {}}))


    #####################
    # PRIVATE - Methods #
    #####################


    def __build_paths_(self, parent, path):
        """
        Recursively adds source code paths to be scanned using the given parent block and path.

        parent : The parent block whose children are scanned for potential source code paths.

        path : The path of the given parent block that is appended to any child block's path added.
        """
        #
        # Iterate through all children blocks of the given parent.
        #
        for block in parent :
            #
            # Check if the child block has a directory name.
            #
            if block.dir_name() is not None :
                #
                # Add the package block's init python script path and then recursively call this
                # method to add all its sub packages and modules.
                #
                self._add_path_(block,os.path.join(path,block.dir_name(),"__init__.py"))
                self.__build_paths_(block,os.path.join(path,block.dir_name()))
            #
            # Else if this child block has a file name and therefore is a module then add its python
            # script path.
            #
            elif block.file_name() is not None :
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
        #
        # Skip any initial doc string of the script.
        #
        self.__skip_doc_string_(ifile)
        #
        # Initialize the empty list of returned import lines.
        #
        ret = []
        #
        # Peek the first code line, making sure it is an import line.
        #
        last = ifile.tell()
        line = ifile.readline()
        ifile.seek(last)
        if not self.__import_pattern.match(line) : return ret
        #
        # Continue while there are import lines to read.
        #
        while True :
            #
            # Read the next line, breaking if it is a new line or adding it to the list of returned
            # import lines otherwise.
            #
            line = ifile.readline()
            if not line or line == '\n' : break
            ret.append(line[:-1])
        #
        # Return the scanned list of import lines.
        #
        return ret


    def __scan_class_(self, ifile):
        """
        Getter method.

        ifile : The python script file whose class at the current seek position is scanned.

        return : Scanned class definition containing any found and scanned method's lines of code
                 from the class definition in the given python script file at its current seek
                 position.
        """
        #
        # Skip any initial doc string of the class.
        #
        self.__skip_doc_string_(ifile)
        #
        # Initialize the empty class scan definition that is returned.
        #
        ret = {"functions":{}}
        #
        # Initialize the scanner with the given file.
        #
        scan = Scanner(ifile)
        #
        # Continue while there are lines to scan within the current class's block of code.
        #
        while True :
            #
            # Read the next line, breaking out if there are no more lines to read for the class.
            #
            line = scan.readline()
            if line is None : break
            #
            # If the line is a function header then scan it and insert its scanned function
            # definition into the class scan definition, using a unique key so it does not overwrite
            # any other scanned functions.
            #
            match = self.__method_pattern.match(line)
            if match :
                uniq_insert(ret["functions"],match.group(1),self.__scan_function_(ifile))
        #
        # Return the scanned class definition.
        #
        return ret


    def __scan_function_(self, ifile):
        """
        Getter method.

        ifile : The python script file whose function at the current seek position is scanned.

        return : Scanned function definition containing all lines of scanned code along with inline
                 comment markers from the function definition in the given python script file at its
                 current seek position.
        """
        #
        # Skip any initial doc string of the function.
        #
        self.__skip_doc_string_(ifile)
        #
        # Initial a special Boolean latch that is used to read in only the first line of any
        # continuous block of comment lines.
        #
        latch = True
        #
        # Initialize the returned list of lines and a scanner with the given file.
        #
        ret = []
        scan = Scanner(ifile)
        #
        # Continue while there are lines to scan within the current function's definition.
        #
        while True :
            #
            # Read the next line, breaking out if there are no more lines to read.
            #
            line = scan.readline()
            if line is None : break
            #
            # Check if the line is a comment.
            #
            match = self.__comment_pattern.match(line)
            if match :
                #
                # If this is the first line of a comment block then add its stripped version to the
                # returned list.
                #
                if latch :
                    ret.append(match.group(1)[scan.indent():])
                    latch = False
            #
            # Else this is a regular line of code so add it with its function indent stripped and
            # reset the latch.
            #
            else:
                ret.append(line[scan.indent():])
                latch = True
        #
        # Return the scanned lines of code and inline comment markers.
        #
        return ret


    def __skip_doc_string_(self, ifile):
        """
        Seeks past any initial doc string starting at the current seek index of the given python
        script file. Blank lines also skipped and ignored.

        ifile : The python script file whose doc string, if any, is skipped.
        """
        #
        # Initialize the doc line count to 0.
        #
        count = 0
        #
        # Continue until both the beginning and ending doc string lines have been read or no doc
        # string was found.
        #
        while count < 2 :
            #
            # Read the next line, keeping track of the last line's seek position.
            #
            last = ifile.tell()
            line = ifile.readline()
            #
            # If the end of file is reached then return.
            #
            if not line : break
            #
            # If this is a doc string line then increment the doc line count.
            #
            if self.__doc_pattern.match(line) : count += 1
            #
            # Else if this is not a blank line and the doc line count is 0 then return.
            #
            elif line != '\n' and count == 0 :
                ifile.seek(last)
                break
