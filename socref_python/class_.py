"""
todo
"""
import html
from PySide2 import QtGui as qtg
from socref import block_factory as bf
from socref import util
from . import package
from . import settings








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








@bf.register_block("Class")
class Builder(Block):


    def space(self, previous, above):
        return "\n"*settings.H1LINES


    def build(self, def_, begin=""):
        if self._BLOCKNAME_ != "Class" : return
        def_ = def_["classes"][self._p_name]
        ret = "%sclass %s():\n" % (begin,self._p_name)
        ret += begin + " "*settings.INDENT + '"""\n'
        ret += util.wrap_text(self._p_description
                              ,begin=begin + " "*settings.INDENT
                              ,columns=settings.COLUMNS)
        ret += begin + " "*settings.INDENT + '"""\n'
        previous = None
        for block in self :
            ret += block.space(previous,self)
            ret += block.build(def_,begin=begin + " "*settings.INDENT)
        return ret
