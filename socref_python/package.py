"""
Detailed description.
"""
import html
from PySide2 import QtGui as qtg
from socref import util
from socref import abstract
from . import parser
from . import settings








class Block(abstract.Block):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self):
        """
        Detailed description.
        """
        abstract.Block.__init__(self)
        #
        self._p_name = ""
        #
        self._p_description = ""
        #


    ####################
    # PUBLIC - Methods #
    ####################


    def parser(self):
        """
        Detailed description.

        return : Yes
        """
        return parser.Parser(self)


    def dir_name(self):
        """
        Detailed description.

        return : Yes
        """
        if self._BLOCKNAME_ == "Package" : return self._p_name


    def icon(self):
        """
        Detailed description.

        return : Yes
        """
        return qtg.QIcon(":/python/package.svg")


    def display_name(self):
        """
        Detailed description.

        return : Yes
        """
        return self._p_name


    def display_view(self):
        """
        Detailed description.

        return : Yes
        """
        description = html.escape(self._p_description)
        if description : description = "<h1>Description</h1><p>%s</p>" % description
        return description


    def build_list(self):
        """
        Detailed description.

        return : Yes
        """
        return ("Package","Module")


    def edit_definitions(self):
        """
        Detailed description.

        return : Yes
        """
        ret = []
        ret.append(util.line_edit("Name:","_p_name"))
        ret.append(util.text_edit("Description:","_p_description",speller=True))
        return ret


    def set_default_properties(self):
        """
        Detailed description.
        """
        self._p_name = "package"
        self._p_description = "Detailed description."


    def clear_properties(self):
        """
        Detailed description.
        """
        self._p_name = ""
        self._p_description = ""








@abstract.register_block("Package",root=True)
class Builder(Block):
    """
    Detailed description.
    """


    ####################
    # PUBLIC - Methods #
    ####################


    def build(self, def_):
        """
        Detailed description.

        def_ : Detailed description.

        return : Yes
        """
        if self._BLOCKNAME_ != "Package" : return
        ret = '"""\n'
        ret += util.wrap_text(self._p_description,columns=settings.COLUMNS)
        ret += '"""\n'
        ret += "\n".join(def_["header"] + [""])
        return ret
