"""
todo
"""
from abc import ABC,abstractmethod






class LogicError(Exception):


    def __init__(self,*args):
        Exception.__init__(self,*args)






class Abstract_Block(ABC):


    @abstractmethod
    def name(self):
        pass


    @abstractmethod
    def build_list():
        pass


    def __init__(self):
        ABC.__init__(self)
        # 
        __children = []
        # 
        __parent = None
        #:


    def __eq__(self,other):
        return self is other


    def __len__(self):
        return len(self.__children)


    def __contains__(self,block):
        for child in self.__children:
            if child is block: return True
        return False


    def __iter__(self):
        return self.__children.__iter__()


    def __getitem__(self,index):
        return self.__children[index]


    def index(self):
        if self.__parent is None: raise LogicError("Cannot get index of block with no parent.")
        return self.__parent.__children.index(self)


    def insert(self,index,block):
        if block.parent() is not None:
            raise LogicError("Block is already a child of another block.")
        self.__children.insert(index,block)
        block.__parent = self


    def pop(self,index):
        orphan = self.__children.pop(index)
        orphan.__parent = None
        return orphan
