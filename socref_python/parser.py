"""
todo
"""
import os
import re
from socref import abstract_parser as ap




def uniq_insert(dict_, key, value):
    if key in dict_ :
        count = 0
        key = key + str(count)
        while key in dict_ :
            count += 1
            key = key[:-1] + str(count)
    dict_[key] = value








class Scan():


    def __init__(self, ifile):
        self.__ifile = ifile
        self.__indent = None


    def indent(self):
        if self.__indent is None :
            raise RuntimeError("indent has not been discovered.")
        return self.__indent


    def readline(self):
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








class Parser(ap.Abstract_Parser):


    def __init__(self, root):
        ap.Abstract_Parser.__init__(self,root)
        self.__defs = {}


    def _build_path_list_(self):
        self.__build_paths_(self._root_block,"")


    def _scan_(self, path):
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
                match = re.match('^def +([a-zA-Z_]+\w*)\([\w,=$ ]*\):',line)
                if match :
                    uniq_insert(def_["functions"],match.group(1),self.__scan_function_(ifile))
            self.__defs[path] = def_


    def _build_(self, block, path):
        return block.build(self.__defs[path])


    def __build_paths_(self, parent, path):
        for block in parent :
            if block.dir_name() is not None :
                self._add_path_(block,os.path.join(path,block.dir_name(),"__init__.py"))
                self.__build_paths_(block,os.path.join(path,block.dir_name()))
            elif block.file_name() is not None :
                self._add_path_(block,os.path.join(path,block.file_name()))


    def __scan_header_(self, ifile):
        self.__skip_doc_string_(ifile)
        ret = []
        while True :
            line = ifile.readline()
            if not line or line == '\n' : break
            ret.append(line[:-1])
        return ret


    def __scan_class_(self, ifile):
        self.__skip_doc_string_(ifile)
        ret = {"functions":{}}
        scan = Scan(ifile)
        while True :
            line = scan.readline()
            if line is None : break
            match = re.match(' *def +([a-zA-Z_]+\w*)\([\w,=$ ]*\):',line)
            if match :
                uniq_insert(ret["functions"],match.group(1),self.__scan_function_(ifile))
        return ret


    def __scan_function_(self, ifile):
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


    def __skip_doc_string_(self, ifile):
        count = 0
        while count < 2 :
            last = ifile.tell()
            line = ifile.readline()
            if not line : break
            if re.match(' *"""',line) : count += 1
            elif line != '\n' and count == 0 :
                ifile.seek(last)
                break
