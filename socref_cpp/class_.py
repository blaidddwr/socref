"""
Contains the class block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import register
from socref import utility as ut
from . import settings
from . import namespace
from . import function








@register("Class")
class Class(function.Templatee):
    """
    This is the class block class. It implements the Socrates' Reference abstract block class. It
    represents a C++ class.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new class block.
        """
        function.Templatee.__init__(self)
        self._p_header = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def is_abstract(self):
        """
        Getter method.

        return : True if this class contains any abstract functions or false otherwise.
        """
        for child in self:
            if child._TYPE_ == "Access" and child.has_abstract():
                return True
        return False


    def is_virtual(self):
        """
        Detailed description.
        """
        for child in self:
            if child._TYPE_ == "Access" and child.has_virtual():
                return True
        return False


    def icon(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        if self.is_abstract():
            return qtg.QIcon(":/cpp/abstract_class.svg")
        elif self.is_virtual():
            return qtg.QIcon(":/cpp/virtual_class.svg")
        else:
            return qtg.QIcon(":/cpp/class.svg")


    def display_name(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return self._template_name_() + self._p_name


    def display_view(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return namespace.Base.display_view(self) + self._templates_view_()


    def build_list(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return ("Template","Access")


    def is_volatile_below(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        return True


    def edit_definitions(self):
        """
        Implements the socref.abstract.AbstractBlock interface.

        return : See interface docs.
        """
        ret = namespace.Base.edit_definitions(self)
        ret.append(ut.textEdit("Header:","_p_header"))
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.set_default_properties(self)
        self._p_name = "class"
        self._p_header = ""


    def clear_properties(self):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        namespace.Base.clear_properties(self)
        self._p_header = ""


    def build_header(self, definitions):
        """
        Detailed description.

        definitions : Detailed description.
        """
        ret = self.__build_header_guard_()
        names = []
        up = self.parent()
        while up is not None:
            if up._TYPE_ == "Namespace" and up.parent():
                names.append(up._p_name)
            up = up.parent()
        names.reverse()
        if names:
            ret += "\n"
            for name in names:
                ret += "namespace " + name + "\n{\n"
        ret += self.build_declaration("",False)
        defs = self.build_header_definitions("",False)
        if defs:
            ret += "\n"*settings.H2LINES + defs
        if names:
            ret += "\n" + "}\n"*len(names)
        ret += "\n#endif\n"
        return ret


    def build_source(self):
        """
        Detailed description.
        """
        pass


    def build_declaration(self, begin, template):
        """
        Detailed description.

        begin : Detailed description.

        template : Detailed description.
        """
        if self.has_templates():
            template = True
        ret = begin + "\n"*settings.H1LINES
        ret += (
            begin
            + "/*!\n"
            + ut.wrapBlocks(self._p_description,begin+" * ",begin+" *",settings.COLUMNS)
            + begin
            + " */\n"
        )
        ret += begin + self.template_declaration() + "class %s\n"%self._p_name
        ret += begin + "{\n"
        header = [begin + " "*settings.INDENT + line for line in self._p_header.split("\n") if line]
        if header:
            ret += "\n".join(header) + "\n"
        for child in self:
            if child._TYPE_ == "Access":
                ret += child.build_header(begin,template)
        ret += begin+"};\n"
        return ret


    def build_definitions(self, begin, template):
        """
        Detailed description.

        begin : Detailed description.

        template : Detailed description.
        """
        pass


    def build_header_definitions(self, scope, template):
        """
        Detailed description.

        scope : Detailed description.

        template : Detailed description.
        """
        if scope:
            scope += "::"
        scope += self._p_name
        if self.has_templates():
            template = True
            scope += self.template_scope()
        ret = ""
        for child in self:
            if child._TYPE_ == "Access":
                ret += child.build_header_definitions(scope,template)
        return ret


    #####################
    # PRIVATE - Methods #
    #####################


    def __build_header_guard_(self):
        """
        Detailed description.
        """
        names = [self._p_name.upper()]
        up = self.parent()
        while up is not None:
            if (
                up._TYPE_ == "Class"
                or (up._TYPE_ == "Namespace" and up.parent() and not up.is_hidden())
            ):
                names.append(up._p_name.upper())
            up = up.parent()
        names.reverse()
        guard = "_".join(names) + "_H"
        return "#ifndef %s\n#define %s\n" % (guard,guard)
