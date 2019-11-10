"""
todo
"""
import html
from PySide2 import QtGui as qtg
from socref import block_factory as bf
from . import package






@bf.register_block("Function")
class Block(package.Block):


    def __init__(self):
        package.Block.__init__(self)
        #:


    def descriptors(self, escape=True):
        ret = self._props["descriptors"]
        if escape : ret = html.escape(ret)
        return ret


    def return_description(self, escape=True):
        ret = self._props["return_description"]
        if escape : ret = html.escape(ret)
        return ret


    def icon(self):
        return qtg.QIcon(":/python/function.svg")


    def display_name(self):
        ret = ""
        if self.return_description() : ret += "... "
        ret += "%s(%i)" % (self._props["name"],len(self))
        if self.descriptors() : ret += " " + "@" * (self.descriptors().count("\n") + 1)
        return ret


    def display_view(self):
        return_description = self.return_description(escape=True)
        if return_description : return_description = "<h2>Return</h2><p>%s</p>" % return_description
        descriptors = self.descriptors(escape=True).replace("\n","<br/>@")
        if descriptors : descriptors = "<h2>Descriptors</h2><p>@%s</p>" % descriptors
        return (package.Block.display_view(self)
                + self._display_arguments_()
                + return_description
                + descriptors)



    def build_list(self):
        return ("Object",)


    def edit_definitions(self):
        ret = package.Block.edit_definitions(self)
        element = dict()
        element["type"] = "text"
        element["label"] = "Return:"
        element["key"] = "return_description"
        ret.append(element)
        element = dict()
        element["type"] = "text"
        element["label"] = "Descriptor(s):"
        element["key"] = "descriptors"
        ret.append(element)
        return ret


    def set_default_properties(self):
        self._props["name"] = "function"
        self._props["description"] = "Detailed description."
        self._props["descriptors"] = ""
        self._props["return_description"] = ""


    def clear_properties(self):
        self._props["name"] = ""
        self._props["description"] = ""
        self._props["descriptors"] = ""
        self._props["return_description"] = ""


    def _display_arguments_(self):
        ret = ""
        for argument in self :
            ret += "<p><b>%s</b> : %s</p>" % (html.escape(argument.name())
                                              ,html.escape(argument.description()))
        if ret : ret = "<h2>Arguments</h2>" + ret
        return ret
