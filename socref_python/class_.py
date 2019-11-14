"""
todo
"""
import html
from PySide2 import QtGui as qtg
from socref import block_factory as bf
from . import package






@bf.register_block("Class")
class Block(package.Block):


    def __init__(self):
        package.Block.__init__(self)
        #:


    def is_abstract(self):
        for access in self :
            if access.has_abstract() : return True
        return False


    def icon(self):
        if self.is_abstract() : return qtg.QIcon(":/python/abstract_class.svg")
        else: return qtg.QIcon(":/python/class.svg")


    def build_list(self):
        return ("Access",)


    def set_default_properties(self):
        self._p_name = "class"
        self._p_description = "Detailed description."
