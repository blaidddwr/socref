"""
todo
"""
from PySide2 import QtGui as qtg
from socref import abstract_block as ab
from socref import block_factory as bf






@bf.register_block("Package",root=True)
class Block(ab.Abstract_Block):


    def __init__(self):
        ab.Abstract_Block.__init__(self,self._LANG_,self._BLOCKNAME_)
        #
        self.__props = {"name":""
                        ,"description":""}
        #:


    def icon(self):
        return qtg.QIcon(":/icons/package.svg")


    def display_name(self):
        return self.__props["name"]


    def display_view(self):
        return f"<h1>Description</h1><p>{self.__props['description']}</p>"


    def build_list(self):
        return ("Package",)


    def edit_definitions(self):
        return [{"type":"line", "label":"Name:", "key":"name"}
                ,{"type":"text", "label":"Description:", "key":"description"}]


    def properties(self):
        return self.__props


    def set_properties(self, props):
        self.__props = props


    def set_default_properties(self):
        self.__props["name"] = "package"
        self.__props["description"] = "Detailed description."


    def clear_properties(self):
        self.__props["name"] = ""
        self.__props["description"] = ""
