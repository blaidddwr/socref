"""
todo
"""
from socref.abstract_block import Abstract_Block
from socref.block_factory import register_block





@register_block("Package")
class Package(Abstract_Block):
    """
    todo
    """

    def name(self):
        """
        todo
        """
        return ""

    def build_list(self):
        """
        todo
        """
        return ("Package")
