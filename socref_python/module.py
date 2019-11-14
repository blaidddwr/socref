"""
todo
"""
from PySide2 import QtGui as qtg
from socref import block_factory as bf
from . import package






@bf.register_block("Module")
class Block(package.Block):


    def __init__(self):
        package.Block.__init__(self)
        #:


    def file_name(self):
        return self._p_name + ".py"


    def icon(self):
        return qtg.QIcon(":/python/module.svg")


    def build_list(self):
        return ("Object","Function","Class")


    def set_default_properties(self):
        self._p_name = "module"
        self._p_description = "Detailed description."
