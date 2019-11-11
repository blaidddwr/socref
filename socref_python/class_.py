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


    def icon(self):
        return qtg.QIcon(":/python/class.svg")


    def build_list(self):
        return ()


    def set_default_properties(self):
        self._p_name = "class"
        self._p_description = "Detailed description."
