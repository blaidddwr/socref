"""
todo
"""
import html
from PySide2 import QtGui as qtg
from socref import abstract_block as ab
from socref import block_factory as bf
from socref import util
from . import function
from . import settings








class Block(ab.Abstract_Block):


    def __init__(self):
        ab.Abstract_Block.__init__(self,self._LANG_,self._BLOCKNAME_)
        #
        self._p_name = ""
        #
        self._p_type = "Public"
        #:


    def has_abstract(self):
        for block in self :
            if isinstance(block,function.Block) :
                if block.is_abstract() : return True
        return False


    def is_volatile_above(self):
        return True


    def icon(self):
        if self._p_type == "Public" : return qtg.QIcon(":/python/public.svg")
        elif  self._p_type == "Protected" : return qtg.QIcon(":/python/protected.svg")
        else:  return qtg.QIcon(":/python/private.svg")


    def display_name(self):
        return self._p_name


    def display_view(self):
        type_ = html.escape(self._p_type.upper())
        name = html.escape(self._p_name)
        return "<h1>%s</h1><p>%s</p>" % (type_,name)


    def build_list(self):
        return ("Object","Function")


    def edit_definitions(self):
        ret = []
        ret.append(util.line_edit("Name:","_p_name"))
        combo = util.combobox_edit("Type:","_p_type")
        util.add_combo_select(combo,"Public",icon=qtg.QIcon(":/python/public.svg"))
        util.add_combo_select(combo,"Protected",icon=qtg.QIcon(":/python/protected.svg"))
        util.add_combo_select(combo,"Private",icon=qtg.QIcon(":/python/private.svg"))
        ret.append(combo)
        return ret


    def set_default_properties(self):
        self._p_name = "access"
        self._p_type = "Public"


    def clear_properties(self):
        self._p_name = ""
        self._p_type = "Public"








@bf.register_block("Access")
class Builder(Block):


    def space(self, previous, above):
        return "\n"*settings.H3LINES


    def build(self, def_, begin=""):
        if self._BLOCKNAME_ != "Access" : return
        line = "# %s - %s #" % (self._p_type.upper(),self._p_name)
        ret = begin + "#"*len(line) + "\n"
        ret += begin + line + "\n"
        ret += begin + "#"*len(line) + "\n"
        previous = None
        for block in self :
            ret += block.space(previous,self)
            ret += block.build(def_,begin=begin)
        return ret
