"""
todo
"""
from socref.abstract_block import Abstract_Block
from socref.block_factory import register_block





@register_block("Package",root=True)
class Package(Abstract_Block):


    def name(self):
        return ""


    def build_list(self):
        return ("Package",)
