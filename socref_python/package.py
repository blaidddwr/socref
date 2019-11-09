"""
todo
"""
from socref.abstract_block import Abstract_Block
from socref.block_factory import register_block





@register_block("Package",root=True)
class Package(Abstract_Block):


    def __init__(self):
        Abstract_Block.__init__(self,self._LANG_,self._BLOCKNAME_)
        #
        self.__props = {"name":""}
        #:


    def display_name(self):
        return self.__props["name"]


    def display_view(self):
        return "Place detailed description here."


    def build_list(self):
        return ("Package",)


    def edit_definitions(self):
        return [{"type":"line", "label":"Name:", "key":"name"}]


    def properties(self):
        return self.__props


    def set_properties(self, props):
        self.__props = props


    def set_default_properties(self):
        self.__props["name"] = "package"


    def clear_properties(self):
        self.__props["name"] = ""
