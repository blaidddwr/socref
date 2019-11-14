"""
todo
"""
import os
from socref import abstract_parser as ap






class Parser(ap.Abstract_Parser):


    def __init__(self, root):
        ap.Abstract_Parser.__init__(self,root)


    def _build_path_list_(self):
        self.__build_paths_(self._root_block,"")


    def _scan_(self, path):
        print(path)


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
