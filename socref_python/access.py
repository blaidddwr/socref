"""
Detailed description.
"""
import html
from PySide2 import QtGui as qtg
from socref import util
from socref import abstract
from . import function
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
        self._p_name = ""
        self._p_type = "Public"


    ####################
    # PUBLIC - Methods #
    ####################


    def has_abstract(self):
        """
        Detailed description.

        return : Yes
        """
        for block in self :
            if isinstance(block,function.Block) :
                if block.is_abstract() : return True
        return False


    def is_volatile_above(self):
        """
        Detailed description.

        return : Yes
        """
        return True


    def icon(self):
        """
        Detailed description.

        return : Yes
        """
        if self._p_type == "Public" : return qtg.QIcon(":/python/public.svg")
        elif  self._p_type == "Protected" : return qtg.QIcon(":/python/protected.svg")
        else:  return qtg.QIcon(":/python/private.svg")


    def display_name(self):
        """
        Detailed description.

        return : Yes
        """
        return self._p_name + " (%i)" % len(self)


    def display_view(self):
        """
        Detailed description.

        return : Yes
        """
        type_ = html.escape(self._p_type.upper())
        name = html.escape(self._p_name)
        return "<h1>%s</h1><p>%s</p>" % (type_,name)


    def build_list(self):
        """
        Detailed description.

        return : Yes
        """
        return ("Object","Function")


    def edit_definitions(self):
        """
        Detailed description.

        return : Yes
        """
        ret = []
        ret.append(util.line_edit("Name:","_p_name"))
        combo = util.combobox_edit("Type:","_p_type")
        util.add_combo_select(combo,"Public",icon=qtg.QIcon(":/python/public.svg"))
        util.add_combo_select(combo,"Protected",icon=qtg.QIcon(":/python/protected.svg"))
        util.add_combo_select(combo,"Private",icon=qtg.QIcon(":/python/private.svg"))
        ret.append(combo)
        return ret


    def set_default_properties(self):
        """
        Detailed description.
        """
        self._p_name = "access"
        self._p_type = "Public"


    def clear_properties(self):
        """
        Detailed description.
        """
        self._p_name = ""
        self._p_type = "Public"








@abstract.register_block("Access")
class Builder(Block):
    """
    Detailed description.
    """


    ####################
    # PUBLIC - Methods #
    ####################


    def space(self, previous, above):
        """
        Detailed description.

        previous : Detailed description.

        above : Detailed description.

        return : Yes
        """
        return "\n"*settings.H3LINES


    def build(self, def_, begin=""):
        """
        Detailed description.

        def_ : Detailed description.

        begin : Detailed description.

        return : Yes
        """
        if self._TYPE_ != "Access" : return
        line = "# %s - %s #" % (self._p_type.upper(),self._p_name)
        ret = begin + "#"*len(line) + "\n"
        ret += begin + line + "\n"
        ret += begin + "#"*len(line) + "\n"
        previous = None
        for block in self :
            ret += block.space(previous,self)
            ret += block.build(def_,begin=begin)
            previous = block
        return ret
