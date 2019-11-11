"""
todo
"""
import html
from PySide2 import QtGui as qtg
from socref import abstract_block as ab
from socref import block_factory as bf






@bf.register_block("Package",root=True)
class Block(ab.Abstract_Block):


    def __init__(self):
        ab.Abstract_Block.__init__(self,self._LANG_,self._BLOCKNAME_)
        #
        self._p_name = ""
        #
        self._p_description = ""
        #:


    def icon(self):
        return qtg.QIcon(":/python/package.svg")


    def display_name(self):
        return self._p_name


    def display_view(self):
        description = html.escape(self._p_description)
        if description : description = "<h1>Description</h1><p>%s</p>" % description
        return description


    def build_list(self):
        return ("Package","Module")


    def edit_definitions(self):
        ret = []
        ret.append(self._get_line_edit_("Name:","_p_name"))
        ret.append(self._get_text_edit_("Description:","_p_description"))
        return ret


    def set_default_properties(self):
        self._p_name = "package"
        self._p_description = "Detailed description."


    def clear_properties(self):
        self._p_name = ""
        self._p_description = ""
