"""
todo
"""
import os
import re
from socref import abstract_parser as ap






class Parser(ap.Abstract_Parser):


    def __init__(self, root):
        ap.Abstract_Parser.__init__(self,root)
        self.__defs = {}


    def _build_path_list_(self):
        self.__build_paths_(self._root_block,"")


    def _scan_(self, path):
        with open(path,"r") as ifile :
            def_ = {"header": self.__scan_header_(ifile)
                    ,"functions": {}}
            while True :
                line = ifile.readline()
                if not line : break
                line = line[:-1]
                match = re.match('^def +([a-zA-Z_]+\w*)\([\w,=$ ]*\):',line)
                if match :
                    key = match.group(1)
                    if key in def_["functions"] :
                        count = 0
                        key = key + str(count)
                        while key in def_["functions"] :
                            count += 1
                            key = key[:-1] + str(count)
                    def_["functions"][key] = self.__scan_function_(ifile)
            self.__defs[path] = def_
            print(def_)


    def _build_(self, block, path):
        print(block,path,sep="\n")
        return ""


    def __build_paths_(self, parent, path):
        for block in parent :
            if block.dir_name() is not None :
                self._add_path_(block,os.path.join(path,block.dir_name(),"__init__.py"))
                self.__build_paths_(block,os.path.join(path,block.dir_name()))
            elif block.file_name() is not None :
                self._add_path_(block,os.path.join(path,block.file_name()))


    def __scan_header_(self, ifile):
        ret = []
        count = 0
        while count < 2 :
            line = ifile.readline()
            if not line : break
            if line == '"""\n' : count += 1
        while True :
            line = ifile.readline()
            if not line or line == '\n' : break
            ret.append(line[:-1])
        return ret


    def __scan_function_(self, ifile):
        ret = []
        indent = None
        while True :
            last = ifile.tell()
            line = ifile.readline()
            if not line : break
            if line != '\n' :
                line = line[:-1]
                current = len(line) - len(line.lstrip(' '))
                if indent is None : indent = current
                elif current < indent :
                    ifile.seek(last)
                    break
                ret.append(line[indent:])
        return ret
