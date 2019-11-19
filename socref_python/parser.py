"""
Detailed description.
"""
import os
import re
from socref import abstract




def uniq_insert(dict_, key, value):
    """
    Detailed description.

    dict_ : Detailed description.

    key : Detailed description.

    value : Detailed description.
    """
    if key in dict_ :
        count = 0
        key = key + str(count)
        while key in dict_ :
            count += 1
            key = key[:-1] + str(count)
    dict_[key] = value








class Scan():
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self, ifile):
        """
        Detailed description.

        ifile : Detailed description.
        """
        self.__ifile = ifile
        self.__indent = None


    ####################
    # PUBLIC - Methods #
    ####################


    def indent(self):
        """
        Detailed description.

        return : Yes
        """
        if self.__indent is None :
            raise RuntimeError("indent has not been discovered.")
        return self.__indent


    def readline(self):
        """
        Detailed description.

        return : Yes
        """
        while True :
            last = self.__ifile.tell()
            line = self.__ifile.readline()
            if not line : return None
            if line != '\n' :
                line = line[:-1]
                indent = len(line) - len(line.lstrip(' '))
                if self.__indent is None : self.__indent = indent
                elif indent < self.__indent :
                    self.__ifile.seek(last)
                    return None
                return line








class Parser(abstract.Parser):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self, root):
        """
        Detailed description.

        root : Detailed description.
        """
        abstract.Parser.__init__(self,root)
        self.__defs = {}


    #######################
    # PROTECTED - Methods #
    #######################


    def _build_path_list_(self):
        """
        Detailed description.
        """
        self.__build_paths_(self._root_block,"")


    def _scan_(self, path):
        """
        Detailed description.

        path : Detailed description.
        """
        with open(path,"r") as ifile :
            def_ = {"header": self.__scan_header_(ifile)
                    ,"functions": {}
                    ,"classes": {}}
            while True :
                line = ifile.readline()
                if not line : break
                line = line[:-1]
                match = re.match('^class +([a-zA-Z_]+\w*)\([\w\._]*\):',line)
                if match : uniq_insert(def_["classes"],match.group(1),self.__scan_class_(ifile))
                match = re.match('^def +([a-zA-Z_]+\w*)\(.*\):',line)
                if match :
                    uniq_insert(def_["functions"],match.group(1),self.__scan_function_(ifile))
            self.__defs[path] = def_


    def _build_(self, block, path):
        """
        Detailed description.

        block : Detailed description.

        path : Detailed description.

        return : Yes
        """
        return block.build(self.__defs.get(path,{"header": [], "functions": {}, "classes": {}}))


    #####################
    # PRIVATE - Methods #
    #####################


    def __build_paths_(self, parent, path):
        """
        Detailed description.

        parent : Detailed description.

        path : Detailed description.
        """
        for block in parent :
            if block.dir_name() is not None :
                self._add_path_(block,os.path.join(path,block.dir_name(),"__init__.py"))
                self.__build_paths_(block,os.path.join(path,block.dir_name()))
            elif block.file_name() is not None :
                self._add_path_(block,os.path.join(path,block.file_name()))


    def __scan_header_(self, ifile):
        """
        Detailed description.

        ifile : Detailed description.

        return : Yes
        """
        self.__skip_doc_string_(ifile)
        ret = []
        last = ifile.tell()
        line = ifile.readline()
        ifile.seek(last)
        if not re.match('(from|import).*',line) : return ret
        while True :
            last = ifile.tell()
            line = ifile.readline()
            if not line or line == '\n' : break
            ret.append(line[:-1])
        return ret


    def __scan_function_(self, ifile):
        """
        Detailed description.

        ifile : Detailed description.

        return : Yes
        """
        self.__skip_doc_string_(ifile)
        ret = []
        latch = True
        scan = Scan(ifile)
        while True :
            line = scan.readline()
            if line is None : break
            match = re.match('( *#).*',line)
            if match :
                if latch :
                    ret.append(match.group(1)[scan.indent():])
                    latch = False
            else:
                ret.append(line[scan.indent():])
                latch = True
        return ret


    def __scan_class_(self, ifile):
        """
        Detailed description.

        ifile : Detailed description.

        return : Yes
        """
        self.__skip_doc_string_(ifile)
        ret = {"functions":{}}
        scan = Scan(ifile)
        while True :
            line = scan.readline()
            if line is None : break
            match = re.match(' *def +([a-zA-Z_]+\w*)\(.*\):',line)
            if match :
                uniq_insert(ret["functions"],match.group(1),self.__scan_function_(ifile))
        return ret


    def __skip_doc_string_(self, ifile):
        """
        Detailed description.

        ifile : Detailed description.

        return : Yes
        """
        count = 0
        while count < 2 :
            last = ifile.tell()
            line = ifile.readline()
            if not line : break
            if re.match(' *"""',line) : count += 1
            elif line != '\n' and count == 0 :
                ifile.seek(last)
                break
