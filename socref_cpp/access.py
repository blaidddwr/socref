"""
Contains the access block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from socref import abstract
from . import settings








@register("Access")
class Block(abstract.Block):
    """
    This is the access block class. It implements the Socrates' Reference abstract block class. It
    represents an access declaration for C++ class.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new access block.
        """
        abstract.Block.__init__(self)
        self._p_name = ""
        self._p_type = "Public"
        self._p_enclosure = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def has_abstract(self):
        """
        Getter method.

        return : True if this access block contains abstract methods or false otherwise.
        """
        for block in self:
            if block._TYPE_ == "Function":
                if block.is_abstract():
                    return True
        return False


    def has_virtual(self):
        """
        Detailed description.
        """
        for block in self:
            if block._TYPE_ == "Function":
                if block.is_virtual():
                    return True
        return False


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        if self._p_type == "Public":
            return qtg.QIcon(":/cpp/public.svg")
        elif  self._p_type == "Protected":
            return qtg.QIcon(":/cpp/protected.svg")
        else:
            return qtg.QIcon(":/cpp/private.svg")


    def display_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return self._p_name + " (%i)" % len(self)


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return "<h1>%s</h1><p>%s</p>" % (
            html.escape(self._p_type.upper())
            ,html.escape(self._p_name)
        )


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ("Variable","Function","Class","Union")


    def is_volatile_above(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return True


    def is_volatile_below(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return True


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        ret = []
        ret.append(ut.line_edit("Name:","_p_name"))
        combo = ut.combobox_edit("Type:","_p_type")
        ut.add_combo_select(combo,"Public",icon=qtg.QIcon(":/cpp/public.svg"))
        ut.add_combo_select(combo,"Protected",icon=qtg.QIcon(":/cpp/protected.svg"))
        ut.add_combo_select(combo,"Private",icon=qtg.QIcon(":/cpp/private.svg"))
        ret.append(combo)
        ret.append(ut.text_edit("Enclosure:","_p_enclosure"))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        self._p_name = "access"
        self._p_type = "Public"
        self._p_enclosure = ""


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        self._p_name = ""
        self._p_type = "Public"
        self._p_enclosure = ""


    def build_header(self, begin, template):
        """
        Detailed description.

        begin : Detailed description.

        template : Detailed description.
        """
        ret = "\n"*settings.H2LINES
        header = []
        footer = []
        if self._p_enclosure:
            blocks = self._p_enclosure.split("\n@\n")
            if blocks:
                header = [line for line in blocks[0].split("\n") if line]
            if len(blocks) > 1:
                footer = [line for line in blocks[1].split("\n") if line]
        ret += begin + "/* %s */\n"%self._p_name
        ret += begin + self._p_type.lower() + ":\n"
        next_begin = begin + " "*settings.INDENT
        if header:
            ret += next_begin + "\n".join(header) + "\n"
        for child in self:
            ret += child.build_declaration(next_begin,template)
        if footer:
            ret += "\n"*settings.H2LINES + next_begin + "\n".join(footer) + "\n"
        return ret


    def build_header_definitions(self, scope, template):
        """
        Detailed description.

        scope : Detailed description.

        template : Detailed description.
        """
        ret = ""
        if template:
            for child in self:
                if child._TYPE_ == "Variable":
                    ret += child.build_definition(scope)
        return ret
