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
class Access(abstract.AbstractBlock):
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
        abstract.AbstractBlock.__init__(self)
        self._p_name = ""
        self._p_type = "Public"
        self._p_enclosure = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def buildHeader(self, begin, template):
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
            ret += child.buildDeclaration(next_begin,template)
        if footer:
            ret += "\n"*settings.H2LINES + next_begin + "\n".join(footer) + "\n"
        return ret


    def buildHeaderDefinitions(self, scope, template):
        """
        Detailed description.

        scope : Detailed description.

        template : Detailed description.
        """
        ret = ""
        if template:
            for child in self:
                if child._TYPE_ == "Variable":
                    ret += child.buildDefinition(scope)
        return ret


    def buildList(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Variable","Function","Class","Union")


    def clearProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        self._p_name = ""
        self._p_type = "Public"
        self._p_enclosure = ""


    def displayName(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return self._p_name + " (%i)" % len(self)


    def displayView(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return "<h1>%s</h1><p>%s</p>" % (
            html.escape(self._p_type.upper())
            ,html.escape(self._p_name)
        )


    def editDefinitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = []
        ret.append(ut.lineEdit("Name:","_p_name"))
        combo = ut.comboEdit("Type:","_p_type")
        ut.addComboSelect(combo,"Public",icon=qtg.QIcon(":/cpp/public.svg"))
        ut.addComboSelect(combo,"Protected",icon=qtg.QIcon(":/cpp/protected.svg"))
        ut.addComboSelect(combo,"Private",icon=qtg.QIcon(":/cpp/private.svg"))
        ret.append(combo)
        ret.append(ut.textEdit("Enclosure:","_p_enclosure"))
        return ret


    def hasAbstract(self):
        """
        Getter method.

        return : True if this access block contains abstract methods or false otherwise.
        """
        for block in self:
            if block._TYPE_ == "Function":
                if block.isAbstract():
                    return True
        return False


    def hasVirtual(self):
        """
        Detailed description.
        """
        for block in self:
            if block._TYPE_ == "Function":
                if block.isVirtual():
                    return True
        return False


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        if self._p_type == "Public":
            return qtg.QIcon(":/cpp/public.svg")
        elif  self._p_type == "Protected":
            return qtg.QIcon(":/cpp/protected.svg")
        else:
            return qtg.QIcon(":/cpp/private.svg")


    def isVolatileAbove(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return True


    def isVolatileBelow(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return True


    def setDefaultProperties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        self._p_name = "access"
        self._p_type = "Public"
        self._p_enclosure = ""
