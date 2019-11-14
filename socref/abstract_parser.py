"""
todo
"""
import abc
import os






class DuplicateError(Exception):


    def __init__(self,*args):
        Exception.__init__(self,*args)






class Abstract_Parser(abc.ABC):


    def __init__(self, root_block):
        abc.ABC.__init__(self)
        self.__root_path = ""
        self._root_block = root_block
        self.__paths = []
        self.__blocks = []


    @abc.abstractmethod
    def _build_path_list_(self):
        pass


    @abc.abstractmethod
    def _scan_(self, path):
        pass


    @abc.abstractmethod
    def _build_(self, block, path):
        pass


    def parse(self):
        if self.__root_path == "" : raise RuntimeError("Root path is not set.")
        self._build_path_list_()
        if len(set(self.__paths)) != len(self.__paths) :
            raise DuplicateError("Duplicate file names generated for parsing. This is caused by two"
                                 " blocks generating an identical file name. Perhaps check for"
                                 " blocks with the same display name and parent block.")
        for path in self.__paths :
            if os.path.isfile(path) : self._scan_(path)
        for path,block in zip(self.__paths,self.__blocks) :
            self.__build_(block,path)


    def set_root_path(self, path):
        if self.__root_path != "" : raise RuntimeError("Root path already set.")
        self.__root_path = path


    def _add_path_(self, block, path):
        if self.__root_path == "" : raise RuntimeError("Root path is not set.")
        self.__paths.append(os.path.join(self.__root_path,path))
        self.__blocks.append(block)


    def __build_(self, block, path):
        if not os.path.exists(os.path.dirname(path)) : os.makedirs(os.path.dirname(path))
        if os.path.exists(path):
            old = open(path,"r").read()
            new = self._build_(block,path)
            #if old != new : open(path,"w").write(new)
        #else: open(path,"w").write(self._build_(block,path))
