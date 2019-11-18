"""
todo
"""
import html
from PySide2 import QtGui as qtg
from socref import util
from socref import abstract
from . import package
from . import access
from . import settings








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
        if self._p_descriptors :
            ret += " " + "@" * len(['' for line in self._p_descriptors.split("\n") if line])
        return ret


    def display_view(self):
        self.__check_flags_()
        return_description = html.escape(self._p_return_description)
        if return_description : return_description = "<h2>Return</h2><p>%s</p>" % return_description
        flags = ""
        if self.is_static() : flags += "<li>Static</li>"
        if self.is_abstract() : flags += "<li>Abstract</li>"
        if flags : flags = "<h2>Flags</h2><ul>%s</ul>" % flags
        descriptors = "<br/>".join(("@" + html.escape(line) for line in self._p_descriptors.split("\n") if line))
        if descriptors : descriptors = "<h2>Descriptors</h2><p>%s</p>" % descriptors
        return (package.Block.display_view(self)
                + self._display_arguments_()
                + return_description
                + flags
                + descriptors)


    def build_list(self):
        return ("Object",)


    def edit_definitions(self):
        ret = package.Block.edit_definitions(self)
        ret.append(util.text_edit("Return:","_p_return_description"))
        if self.is_method() :
            ret.append(util.checkbox_edit("Static","_p_static"))
            ret.append(util.checkbox_edit("Abstract","_p_abstract"))
        else:
            ret.append(util.hidden_edit("_p_static","0"))
            ret.append(util.hidden_edit("_p_abstract","0"))
        ret.append(util.text_edit("Inline Comments:","_p_inlines"))
        ret.append(util.text_edit("Descriptor(s):","_p_descriptors"))
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


    def _display_arguments_(self):
        ret = ""
        for arg in self : ret += arg.argument_view()
        if ret : ret = "<h2>Arguments</h2>" + ret
        return ret


    def __check_flags_(self):
        if not self.is_method() :
            self._p_static = "0"
            self._p_abstract = "0"








@abstract.register_block("Function")
class Builder(Block):


    def space(self, previous, above):
        if above._BLOCKNAME_ == "Module" :
            if previous is not None and previous._BLOCKNAME_ == "Class" :
                return "\n"*settings.H1LINES
            else: return "\n"*settings.H2LINES
        elif above._BLOCKNAME_ == "Access" : return "\n"*settings.H3LINES
        else: raise RuntimeError("Unkonwn block type.")


    def build(self, def_, begin=""):
        if self._BLOCKNAME_ != "Function" : return
        ret = self.__build_header_(begin)
        ret += self.__build_doc_string_(begin)
        ret += self.__build_lines_(def_["functions"].get(self._p_name,[]),begin)
        return ret


    def __build_header_(self, begin):
        ret = "\n".join((begin + "@" + line for line in self._p_descriptors.split("\n") if line))
        if ret : ret += "\n"
        if self.is_abstract() : ret += begin + "@abc.abstractmethod\n"
        arguments = [arg.argument() for arg in self]
        if self.is_method() and not self.is_static() : arguments.insert(0,"self")
        ret += "%sdef %s(%s):\n" % (begin,self._p_name,", ".join(arguments))
        return ret


    def __build_doc_string_(self, begin):
        ret = begin + " "*settings.INDENT + '"""\n'
        ret += util.wrap_text(self._p_description
                              ,begin=begin + " "*settings.INDENT
                              ,columns=settings.COLUMNS)
        for arg in self : ret += "\n" + arg.comment(begin + " "*settings.INDENT)
        if self._p_return_description :
            initial = "return : "
            ret += "\n" + util.wrap_text(initial + self._p_return_description
                                         ,begin=begin + " "*settings.INDENT
                                         ,after=" "*len(initial)
                                         ,columns=settings.COLUMNS)
        ret += '%s%s"""\n' % (begin," "*settings.INDENT)
        return ret


    def __build_lines_(self, lines, begin):
        if not lines : return begin + " " * settings.INDENT + "pass\n"
        ret = ""
        inlines = [line for line in self._p_inlines.split("\n\n") if line]
        for line in lines :
            if line.endswith("#") :
                if inlines :
                    ret += begin + " " * settings.INDENT + line + "\n"
                    ret += util.wrap_text(inlines.pop(0)
                                          ,begin=begin + " " * settings.INDENT + line + " "
                                          ,columns=settings.COLUMNS)
                    ret += begin + " " * settings.INDENT + line + "\n"
                else: ret += begin + " " * settings.INDENT + line + "\n"
            else: ret += begin + " " * settings.INDENT + line + "\n"
        return ret
