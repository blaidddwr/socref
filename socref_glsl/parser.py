"""
Contains the GLSL language parser class.
"""
import os
import re
from socref import utility as ut
from socref import abstract








class Parser(abstract.AbstractParser):
    """
    This is the GLSL parser class. It implements the Socrates' Reference abstract parser.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self, root):
        """
        Initializes a new GLSL parser with the given root program block.

        root : A program block that is the root block of a GLSL project that this new parser will
               parse.
        """
        abstract.AbstractParser.__init__(self)
        self.__root = root
        self.__functionPattern = re.compile('^.*\s+(\w+)(\(.*)$')
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
        for path in self.__definitions:
            functions = self.__definitions[path]["functions"]
            for fname in functions:
                ret[path+":"+fname] = "\n".join(functions[fname])+"\n"
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
        return "\n".join(block.build(self.__definitions[path]))+"\n"


    def _buildPathList_(self):
        """
        Implements the socref.abstract.AbstractParser interface.
        """
        self.__buildPaths_(self.__root,"")


    def _scan_(self, path):
        """
        Implements the socref.abstract.AbstractParser interface.

        path : See interface docs.
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
                        ut.uniqueInsert(
                            def_["functions"]
                            ,match.group(1)
                            ,self.__scanFunction_(ifile,match.group(2))
                        )
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
            if block._TYPE_ == "Program":
                self.__buildPaths_(block,os.path.join(path,block._p_name))
            elif block._TYPE_ == "Shader":
                self._addPath_(block,os.path.join(path,block._p_name+block.extension()))


    def __scanFunction_(self, ifile, end):
        """
        Getter method.

        ifile : The input file positioned right after a function declaration whose scanned lines are
                returned.

        end : The end part of function declaration, beginning with the first opening parenthesis
              after the function name.

        return : A list of scanned code lines from the given input file, assuming it is positioned
                 right after the function declaration line.
        """
        depth = end.count("(")-end.count(")")
        while depth > 0:
            line = ifile.readline()
            if not line:
                return []
            depth += line.count("(")-line.count(")")
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


    def __scanHeader_(self, ifile):
        """
        Getter method.

        ifile : The input file whose shader directive code lines are scanned.

        return : A list of shader directive code lines scanned from the header of the given shader
                 file.
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
