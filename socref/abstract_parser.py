"""
todo
"""
import abc






class Abstract_Parser(abc.ABC):


    def __init__(self, root_path, root_block):
        abc.ABC.__init__(self)
        self._root_path = root_path
        self._root_block = root_block
        self._paths = []


    @abc.abstractmethod
    def _build_path_list_(self):
        pass


    def _add_path_(self, path):
        pass
