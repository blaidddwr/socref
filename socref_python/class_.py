"""
Detailed description.
"""
import html
from PySide2 import QtGui as qtg
from socref import util
from socref import abstract
from . import package
from . import settings








class Block(package.Block):
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
        package.Block.__init__(self)
        self._p_parents = ""
        self._p_descriptors = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def is_abstract(self):
        """
        Detailed description.

        return : Yes
        """
        for access in self :
            if access.has_abstract() : return True
        return False


    def icon(self):
        """
        Detailed description.

        return : Yes
        """
        if self.is_abstract() : return qtg.QIcon(":/python/abstract_class.svg")
        else: return qtg.QIcon(":/python/class.svg")


    def display_view(self):
        """
        Detailed description.

        return : Yes
        """
        parents = "</li><li>".join((parent for parent in self._p_parents.split("\n") if parent))
        if parents : parents = "<h2>Parents</h2><ul><li>%s</li></ul>" % parents
        descriptors = "<br/>".join(["@" + html.escape(line) for line in self._p_descriptors.split("\n") if line])
        if descriptors : descriptors = "<h2>Descriptors</h2><p>%s</p>" % descriptors
        return package.Block.display_view(self) + parents + descriptors


    def build_list(self):
        """
        Detailed description.

        return : Yes
        """
        return ("Access",)


    def edit_definitions(self):
        """
        Detailed description.

        return : Yes
        """
        ret = package.Block.edit_definitions(self)
        ret.append(util.text_edit("Parents:","_p_parents"))
        ret.append(util.text_edit("Descriptor(s):","_p_descriptors"))
        return ret


    def set_default_properties(self):
        """
        Detailed description.
        """
        package.Block.set_default_properties(self)
        self._p_name = "class"
        self._p_parents = ""
        self._p_descriptors = ""


    def clear_properties(self):
        """
        Detailed description.
        """
        package.Block.clear_properties(self)
        self._p_parents = ""
        self._p_descriptors = ""








@abstract.register_block("Class")
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
        return "\n"*settings.H1LINES


    def build(self, def_, begin=""):
        """
        Detailed description.

        def_ : Detailed description.

        begin : Detailed description.

        return : Yes
        """
        if self._BLOCKNAME_ != "Class" : return
        def_ = def_["classes"].get(self._p_name,{"functions":{}})
        ret = "\n".join((begin + "@" + line for line in self._p_descriptors.split("\n") if line))
        if ret : ret += "\n"
        ret += "%sclass %s(%s):\n" % (begin,self._p_name,self.__build_parents_())
        ret += begin + " "*settings.INDENT + '"""\n'
        ret += util.wrap_text(self._p_description
                              ,begin=begin + " "*settings.INDENT
                              ,columns=settings.COLUMNS)
        ret += begin + " "*settings.INDENT + '"""\n'
        previous = None
        for block in self :
            ret += block.space(previous,self)
            ret += block.build(def_,begin=begin + " "*settings.INDENT)
            previous = block
        return ret


    #####################
    # PRIVATE - Methods #
    #####################


    def __build_parents_(self):
        """
        Detailed description.

        return : Yes
        """
        return ", ".join((parent for parent in self._p_parents.split("\n") if parent))
