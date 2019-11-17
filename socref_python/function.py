"""
todo
"""
import html
from PySide2 import QtGui as qtg
from socref import block_factory as bf
from . import package
from . import access
from . import settings






@bf.register_block("Function")
class Block(package.Block):


    def __init__(self):
        package.Block.__init__(self)
        self._p_descriptors = ""
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_static = "0"
        self._p_abstract = "0"


    def is_static(self):
        return bool(int(self._p_static))


    def is_abstract(self):
        return bool(int(self._p_abstract))


    def is_method(self):
        return isinstance(self.parent(),access.Block)


    def icon(self):
        if self._p_name.startswith("__") and self._p_name.endswith("__") : return qtg.QIcon(":/python/operator.svg")
        elif self.is_static() : return qtg.QIcon(":/python/static_function.svg")
        elif self.is_abstract() : return qtg.QIcon(":/python/abstract_function.svg")
        else: return qtg.QIcon(":/python/function.svg")


    def display_name(self):
        ret = ""
        if self._p_return_description : ret += "... "
        ret += "%s(%i)" % (self._p_name,len(self))
        if self._p_descriptors : ret += " " + "@" * (self._p_descriptors.count("\n") + 1)
        return ret


    def display_view(self):
        self.__check_flags_()
        return_description = html.escape(self._p_return_description)
        if return_description : return_description = "<h2>Return</h2><p>%s</p>" % return_description
        flags = ""
        if self.is_static() : flags += "<li>Static</li>"
        if self.is_abstract() : flags += "<li>Abstract</li>"
        if flags : flags = "<h3>Flags</h3><ul>%s</ul>" % flags
        descriptors = html.escape(self._p_descriptors).replace("\n","<br/>@")
        if descriptors : descriptors = "<h2>Descriptors</h2><p>@%s</p>" % descriptors
        return (package.Block.display_view(self)
                + self._display_arguments_()
                + return_description
                + flags
                + descriptors)


    def build_list(self):
        return ("Object",)


    def edit_definitions(self):
        ret = package.Block.edit_definitions(self)
        ret.append(self._text_edit_("Return:","_p_return_description"))
        if self.is_method() :
            ret.append(self._checkbox_edit_("Static","_p_static"))
            ret.append(self._checkbox_edit_("Abstract","_p_abstract"))
        else :
            ret.append(self._hidden_("_p_static","0"))
            ret.append(self._hidden_("_p_abstract","0"))
        ret.append(self._text_edit_("Inline Comments:","_p_inlines"))
        ret.append(self._text_edit_("Descriptor(s):","_p_descriptors"))
        return ret


    def set_default_properties(self):
        self._p_name = "function"
        self._p_description = "Detailed description."
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_descriptors = ""
        self._p_static = "0"
        self._p_abstract = "0"


    def clear_properties(self):
        self._p_name = ""
        self._p_description = ""
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_descriptors = ""
        self._p_static = "0"
        self._p_abstract = "0"


    def space(self, previous, above):
        return "\n"*settings.H1LINES


    def build(self, def_, begin=""):
        if self._BLOCKNAME_ != "Function" : return
        inlines = self._p_inlines.split("\n\n") if self._p_inlines else []
        lines = def_["functions"].get(self._p_name,[])
        ret = "%sdef %s(%s):\n" % (begin,self._p_name,", ".join((arg.argument() for arg in self)))
        ret += '%s%s"""\n' % (begin," "*settings.INDENT)
        ret += self._wrap_text_(self._p_description
                                ,lead=begin + " "*settings.INDENT
                                ,columns=settings.COLUMNS)
        for arg in self : ret += "\n" + arg.comment(begin + " "*settings.INDENT)
        if self._p_return_description :
            ret += "\n" + self._wrap_text_(self._p_return_description
                                           ,lead=begin + " "*settings.INDENT
                                           ,columns=settings.COLUMNS)
        ret += '%s%s"""\n' % (begin," "*settings.INDENT)
        for line in lines :
            if line.endswith("#") :
                if inlines :
                    ret += "%s%s\n" % (" "*settings.INDENT,line)
                    ret += self._wrap_text_(inlines.pop(0)
                                            ,lead="%s%s " % (" "*settings.INDENT,line)
                                            ,columns=settings.COLUMNS)
                    ret += "%s%s\n" % (" "*settings.INDENT,line)
                else : ret += "%s%s\n" % (" "*settings.INDENT,line)
            else : ret += "%s%s%s\n" % (begin," "*settings.INDENT,line)
        return ret


    def _display_arguments_(self):
        ret = ""
        for argument in self :
            ret += "<p><b>%s</b> : %s</p>" % (html.escape(argument._p_name)
                                              ,html.escape(argument._p_description))
        if ret : ret = "<h2>Arguments</h2>" + ret
        return ret


    def __check_flags_(self):
        if not self.is_method() :
            self._p_static = "0"
            self._p_abstract = "0"
