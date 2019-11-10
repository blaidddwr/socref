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
        self._props = dict()
        #:


    def name(self, escape=False):
        ret = self._props["name"]
        if escape : ret = html.escape(ret)
        return ret


    def description(self, escape=False):
        ret = self._props["description"]
        if escape : ret = html.escape(ret)
        return ret


    def icon(self):
        return qtg.QIcon(":/python/package.svg")


    def display_name(self):
        return self._props["name"]


    def display_view(self):
        description = self.description(escape=True)
        if description : description = "<h1>Description</h1><p>%s</p>" % description
        return description


    def build_list(self):
        return ("Package","Module")


    def edit_definitions(self):
        ret = []
        element = dict()
        element["type"] = "line"
        element["label"] = "Name:"
        element["key"] = "name"
        ret.append(element)
        element = dict()
        element["type"] = "text"
        element["label"] = "Description:"
        element["key"] = "description"
        ret.append(element)
        return ret


    def properties(self):
        return self._props


    def set_properties(self, props):
        self._props = props


    def set_default_properties(self):
        self._props["name"] = "package"
        self._props["description"] = "Detailed description."


    def clear_properties(self):
        self._props["name"] = ""
        self._props["description"] = ""
