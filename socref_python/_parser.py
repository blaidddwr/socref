"""
Contains the Parser class.
"""
import os
import re
from socref import abstract
from socref import edit
from . import parser
from . import settings








class Parser(abstract.AbstractParser):
    """
    This is the python parser class. It implements the Socrates' Reference
    abstract parser. When scanning source code it builds a definitions
    dictionary whose keys are the path of the source code file scanned and
    values are another dictionary for that file containing five keys. The first
    key is "pre" and contains special comment lines before the doc string. The
    second key is "headers" and contains lines of import statements at the head
    of the python file. The third key is "script" and contains any scripting
    lines at the end of the python file after the script header comment. The
    fourth key is "functions" and is a dictionary whose keys are function names
    and values are their scanned lines of code. The fifth key is "classes" that
    is a dictionary whose keys are class names and values are dictionaries with
    one key "functions" that follows the same structure as the second
    "functions" key but for the methods of the class it is contained within.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(
        self
        ,root
        ):
        """
        Initializes a new python parser with the given root package block.

        Parameters
        ----------
        root : socref_python.block.Package
               A package block that is the root block of a python project that
               this new parser will parse.
        """
        abstract.AbstractParser.__init__(self)
        self.__rootBlock = root
        self.__prePattern = re.compile('^ *#.*')
        self.__docPattern = re.compile('^ *"""')
        self.__importPattern = re.compile('^(from|import).*')
        self.__classPattern = re.compile('^class +([a-zA-Z_]+\w*)\([\w\._,\s]*\):')
        self.__functionPattern = re.compile('^def +([a-zA-Z_]+\w*)\((.*)')
        self.__functionEndPattern = re.compile('^ *\):')
        self.__commentPattern = re.compile('^( *#).*')
        self.__methodPattern = re.compile('^ *def +([a-zA-Z_]+\w*)\((.*)')
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
        for key in self.__definitions:
            definition = self.__definitions[key]
            if "header" in definition:
                ret[key + ".header"] = "\n".join(definition["header"]) + "\n"
            self.__addUnknownFunctions_(ret,key,definition["functions"])
            for ckey in definition["classes"]:
                class_ = definition["classes"][ckey]
                self.__addUnknownFunctions_(ret,key + "." + ckey,class_["functions"])
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
            "\n".join(
                block.build(
                    self.__definitions.get(path,{"header": [], "functions": {}, "classes": {}})
                )
            )
            + "\n"
        )


    def _buildPathList_(
        self
        ):
        """
        Implements the socref.abstract.AbstractParser interface.
        """
        self.__buildPaths_(self.__rootBlock,"")


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
            def_ = {
                "pre": []
                ,"header": []
                ,"script": []
                ,"functions": {}
                ,"classes": {}
            }
            def_["pre"] = self.__scanPre_(ifile)
            def_["header"] = self.__scanHeader_(ifile)
            while True:
                line = ifile.readline()
                if not line:
                    break
                line = line[:-1]
                if line==settings.SCRIPT_HEADER:
                    def_["script"] = self.__scanScript_(ifile)
                else:
                    match = self.__classPattern.match(line)
                    if match:
                        edit.uniqueInsert(def_["classes"],match.group(1),self.__scanClass_(ifile))
                    else:
                        match = self.__functionPattern.match(line)
                        if match:
                            edit.uniqueInsert(
                                def_["functions"]
                                ,match.group(1)
                                ,self.__scanFunction_(ifile,match.group(2))
                            )
            self.__definitions[path] = def_


    #####################
    # PRIVATE - Methods #
    #####################


    def __addUnknownFunctions_(
        self
        ,code
        ,root_key
        ,definition
        ):
        """
        Adds unknown code fragment strings to the given dictionary using the
        given root key for any remaining function lines in the given definition
        of functions.

        Parameters
        ----------
        code : dictionary
               Unknown code fragments that has any unused function lines added
               to it.
        root_key : string
                   Appended with the function name and used as the key for
                   inserting any unused function definition found.
        definition : dictionary
                     Function definitions generated from scanning and used by
                     builders.
        """
        for key in definition:
            function = definition[key]
            if function:
                code[root_key + "." + key] = "\n".join(function) + "\n"


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
            if block._TYPE_ == "Package":
                self._addPath_(block,os.path.join(path,block._p_name,"__init__.py"))
                self.__buildPaths_(block,os.path.join(path,block._p_name))
            elif block._TYPE_ == "Module":
                self._addPath_(block,os.path.join(path,block._p_name+".py"))
                self.__buildPaths_(block,path)
            elif block._TYPE_ == "Class" and not block.isInfile():
                self._addPath_(block,os.path.join(path,"_"+block._p_name.lower()+".py"))


    def __scanClass_(
        self
        ,ifile
        ):
        """
        Getter method.

        Parameters
        ----------
        ifile : io.TextIOWrapper
                The python script whose class at the current seek position is
                scanned.

        Returns
        -------
        ret0 : dictionary
               Scanned class definition containing any found and scanned
               method's lines of code from the class definition in the given
               python script file at its current seek position.
        """
        self.__skipDocString_(ifile)
        ret = {"functions": {}}
        scan = parser.Scanner(ifile)
        while True:
            line = scan.readline()
            if line is None:
                break
            match = self.__methodPattern.match(line)
            if match:
                edit.uniqueInsert(
                    ret["functions"]
                    ,match.group(1)
                    ,self.__scanFunction_(ifile,match.group(2))
                )
        return ret


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
                The python script whose function at the current seek position is
                scanned.
        end : string
              The end of the function declaration, beginning after the opening
              parenthesis character.

        Returns
        -------
        ret0 : list
               Scanned function definition containing all lines of scanned code
               from the function definition in the given python script file at
               its current seek position.
        """
        if not end.strip().endswith("):"):
            line = ifile.readline()
            while not self.__functionEndPattern.match(line):
                if not line:
                    break
                line = ifile.readline()
        self.__skipDocString_(ifile)
        ret = []
        scan = parser.Scanner(ifile)
        while True:
            line = scan.readline()
            if line is None:
                break
            ret.append(line[scan.indent():])
        return ret


    def __scanHeader_(
        self
        ,ifile
        ):
        """
        Getter method.

        Parameters
        ----------
        ifile : io.TextIOWrapper
                The python script whose import lines of code are scanned.

        Returns
        -------
        ret0 : list
               Import code lines scanned from the header of the given python
               script file. It is assumed all import lines occur at the
               beginning of the file right after any initial doc string. It is
               also assumed the import lines end with at least one blank line.
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


    def __scanPre_(
        self
        ,ifile
        ):
        """
        Getter method.

        Parameters
        ----------
        ifile : io.TextIOWrapper
                The python script whose comment script lines are scanned.

        Returns
        -------
        ret0 : list
               Comment script lines scanned from the beginning of the given
               python script file.
        """
        ret = []
        while True:
            last = ifile.tell()
            line = ifile.readline()
            if not line:
                break
            if not self.__prePattern.match(line):
                ifile.seek(last)
                break
            ret.append(line[:-1])
        return ret


    def __scanScript_(
        self
        ,ifile
        ):
        """
        Getter method.

        Parameters
        ----------
        ifile : io.TextIOWrapper
                The python script whose script lines are scanned.

        Returns
        -------
        ret0 : list
               Script code lines scanned from the end of the given python script
               file. It is assumed the next line to read is right after the
               script header comment.
        """
        ret = []
        while True:
            line = ifile.readline()
            if not line:
                break
            ret.append(line[:-1])
        return ret


    def __skipDocString_(
        self
        ,ifile
        ):
        """
        Seeks past any initial doc string starting at the current seek index of
        the given python script file. Blank lines also skipped and ignored.

        Parameters
        ----------
        ifile : io.TextIOWrapper
                The python script whose doc string, if any, is skipped.
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
