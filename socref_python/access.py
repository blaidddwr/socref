"""
todo
"""
import html
from PySide2 import QtGui as qtg
from socref import abstract_block as ab
from socref import block_factory as bf
from . import function






@bf.register_block("Access")
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
        else :  return qtg.QIcon(":/python/private.svg")


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
        ret.append(self._get_line_edit_("Name:","_p_name"))
        combo = self._get_combobox_edit_("Type:","_p_type")
        self._add_combo_select_(combo,"Public",icon=qtg.QIcon(":/python/public.svg"))
        self._add_combo_select_(combo,"Protected",icon=qtg.QIcon(":/python/protected.svg"))
        self._add_combo_select_(combo,"Private",icon=qtg.QIcon(":/python/private.svg"))
        ret.append(combo)
        return ret


    def set_default_properties(self):
        self._p_name = "access"
        self._p_type = "Public"


    def clear_properties(self):
        self._p_name = ""
        self._p_type = "Public"
