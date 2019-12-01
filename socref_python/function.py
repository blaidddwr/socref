"""
Contains the function block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import util
from socref import abstract
from . import settings
from . import package
from . import access








class Descriptor(package.Block):
    """
    This is the python descriptor class. It inherits the python package block. It partially
    implements the Socrates' Reference abstract block class. This provides a descriptors property
    along with utility methods for it. This is meant to act as a base class for any block that has
    descriptors.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new descriptor block.
        """
        package.Block.__init__(self)
        #
        # Initialize this block's properties.
        #
        self._p_descriptors = ""


    #############################
    # PUBLIC - Property Methods #
    #############################


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        package.Block.set_default_properties(self)
        self._p_descriptors = ""


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        package.Block.clear_properties(self)
        self._p_descriptors = ""


    #######################
    # PROTECTED - Methods #
    #######################


    def _descriptors_name_(self):
        """
        Getter method.

        return : A string that is a decoration for a block's display name providing information
                 about its descriptors. If this block has no descriptors then an empty string is
                 returned.
        """
        if self._p_descriptors :
            return " " + "@" * len(['' for line in self._p_descriptors.split("\n") if line])
        else: return ""


    def _descriptors_view_(self):
        """
        Getter method.

        return : A rich text string providing detailed information about this block's descriptors.
                 If this block has no descriptors then an empty string is returned.
        """
        ret = "<br/>".join(("@" + html.escape(line) for line in self._p_descriptors.split("\n") if line))
        if ret : ret = "<h2>Descriptors</h2><p>%s</p>" % ret
        return ret


    def _descriptors_edit_definition_(self):
        """
        Getter Method.

        return : The edit definition for this block's descriptors property.
        """
        return util.text_edit("Descriptor(s):","_p_descriptors")


    def _build_descriptors_(self, begin):
        """
        Getter method.

        begin : The indent or begin string that is appended to every line of returned code.

        return : A string that is the source code for this block's descriptors. If this block has no
                 descriptors an empty string is returned.
        """
        ret = "\n".join((begin + "@" + line for line in self._p_descriptors.split("\n") if line))
        if ret : ret += "\n"
        return ret








@abstract.register_block("Function")
class Block(Descriptor):
    """
    This is the function block class. It inherits the python package block. It implements the
    Socrates' Reference abstract block class. It represents a python function.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new function block.
        """
        Descriptor.__init__(self)
        #
        # Initialize this block's properties.
        #
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_static = "0"
        self._p_abstract = "0"


    ##########################
    # PUBLIC - Basic Methods #
    ##########################


    def is_static(self):
        """
        Getter method.

        return : True if this function is static or false otherwise.
        """
        return bool(int(self._p_static))


    def is_abstract(self):
        """
        Getter method.

        return : True if this function is abstract or false otherwise.
        """
        return bool(int(self._p_abstract))


    def is_method(self):
        """
        Getter method.

        return : True if this function is a method of a class or false otherwise.
        """
        return isinstance(self.parent(),access.Block)


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        #
        # Return the appropriate icon based off this function's properties.
        #
        if self._p_name.startswith("__") and self._p_name.endswith("__") : return qtg.QIcon(":/python/operator.svg")
        elif self.is_static() : return qtg.QIcon(":/python/static_function.svg")
        elif self.is_abstract() : return qtg.QIcon(":/python/abstract_function.svg")
        else: return qtg.QIcon(":/python/function.svg")


    def display_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        #
        # Initialize the empty name.
        #
        ret = ""
        #
        # If this function has a return value then append a return indicator.
        #
        if self._p_return_description : ret += "... "
        #
        # Append this function's name, number of arguments, and descriptor decoration.
        #
        ret += "%s(%i)%s" % (self._p_name,len(self),self._descriptors_name_())
        #
        # Return the display name.
        #
        return ret


    def display_view(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        #
        # Check this block's flag properties.
        #
        self.__check_flags_()
        #
        # Generate the rich text return description of this block.
        #
        return_description = html.escape(self._p_return_description)
        if return_description : return_description = "<h2>Return</h2><p>%s</p>" % return_description
        #
        # Generate the rich text flags view of this block.
        #
        flags = ""
        if self.is_static() : flags += "<li>Static</li>"
        if self.is_abstract() : flags += "<li>Abstract</li>"
        if flags : flags = "<h2>Flags</h2><ul>%s</ul>" % flags
        #
        # Return the description, arguments view, return description, flags view, and descriptors
        # view.
        #
        return (package.Block.display_view(self)
                + self._display_arguments_()
                + return_description
                + flags
                + self._descriptors_view_())


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ("Object",)


    #############################
    # PUBLIC - Property Methods #
    #############################


    def edit_definitions(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        #
        # Initialize a list with the inherited package block's edit definitions.
        #
        ret = package.Block.edit_definitions(self)
        #
        # Add the return description edit definition.
        #
        ret.append(util.text_edit("Return:","_p_return_description",speller=True))
        #
        # If this function is a method then add the flag edit definitions, else add hidden
        # definitions for the flag properties.
        #
        if self.is_method() :
            ret.append(util.checkbox_edit("Static","_p_static"))
            ret.append(util.checkbox_edit("Abstract","_p_abstract"))
        else:
            ret.append(util.hidden_edit("_p_static","0"))
            ret.append(util.hidden_edit("_p_abstract","0"))
        #
        # Add the inline comments edit definition and then the descriptors edit definition.
        #
        ret.append(util.text_edit("Inline Comments:","_p_inlines",speller=True))
        ret.append(self._descriptors_edit_definition_())
        #
        # Return the list.
        #
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        Descriptor.set_default_properties(self)
        self._p_name = "function"
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_static = "0"
        self._p_abstract = "0"


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        Descriptor.clear_properties(self)
        self._p_return_description = ""
        self._p_inlines = ""
        self._p_static = "0"
        self._p_abstract = "0"


    ############################
    # PUBLIC - Parsing Methods #
    ############################


    def space(self, previous):
        """
        Implements the .package.Block interface.

        previous : See interface docs.

        return : See interface docs.
        """
        #
        # Initialize the empty string of blank lines.
        #
        ret = ""
        #
        # Set the appropriate number of blank lines based off the given previous block and if this
        # is a method.
        #
        if self.is_method() : ret = "\n"*settings.H3LINES
        else:
            if previous is not None and previous._TYPE_ == "Class" : ret = "\n"*settings.H1LINES
            else: ret = "\n"*settings.H2LINES
        #
        # Return the determined number of blank lines.
        #
        return ret


    def build(self, def_, begin=""):
        """
        Implements the .package.Block interface.

        def_ : See interface docs.

        begin : See interface docs.

        return : See interface docs.
        """
        #
        # Add the header, doc string, and then lines of this function to the returned source code.
        #
        ret = self.__build_header_(begin)
        ret += self.__build_doc_string_(begin)
        ret += self.__build_lines_(def_["functions"].get(self._p_name,[]),begin)
        #
        # Return the source code.
        #
        return ret


    #######################
    # PROTECTED - Methods #
    #######################


    def _display_arguments_(self):
        """
        Getter method.

        return : Rich text view of all this function's arguments. If this function has no arguments
                 then this returns an empty string.
        """
        ret = ""
        for arg in self : ret += arg.argument_view()
        if ret : ret = "<h2>Arguments</h2>" + ret
        return ret


    #####################
    # PRIVATE - Methods #
    #####################


    def __check_flags_(self):
        """
        Sets this function's flags to legal values if it is not a method.
        """
        if not self.is_method() :
            self._p_static = "0"
            self._p_abstract = "0"


    def __build_header_(self, begin):
        """
        Getter method.

        begin : The indent or begin string that is appended to every line of returned code.

        return : A string that is the source code of this function's header. The header includes any
                 descriptors and the define line.
        """
        #
        # Add this function's list of descriptors to the source code.
        #
        ret = self._build_descriptors_(begin)
        #
        # Add the special abstract descriptor to the source code if this function is an abstract
        # method.
        #
        if self.is_abstract() : ret += begin + "@%s\n" % settings.ABSTRACT_DESCRIPTOR
        #
        # Build the list of argument source code fragments of this function.
        #
        arguments = [arg.argument() for arg in self]
        if self.is_method() and not self.is_static() : arguments.insert(0,"self")
        #
        # Add the define line of this function to the source code.
        #
        ret += "%sdef %s(%s):\n" % (begin,self._p_name,", ".join(arguments))
        #
        # Return this function's header source code.
        #
        return ret


    def __build_doc_string_(self, begin):
        """
        Getter method.

        begin : The indent or begin string that is appended to every line of returned code.

        return : A string that is the source code doc string of this function.
        """
        #
        # Add the begin doc string tag to the source code.
        #
        ret = begin + " "*settings.INDENT + '"""\n'
        #
        # Add this function's description to the doc string.
        #
        ret += util.wrap_blocks(self._p_description
                                ,begin=begin + " "*settings.INDENT
                                ,columns=settings.COLUMNS)
        #
        # Add all argument doc string fragments of this function.
        #
        for arg in self : ret += "\n" + arg.comment(begin + " "*settings.INDENT)
        #
        # If this function has a return description then add it to the doc string.
        #
        if self._p_return_description :
            initial = "return : "
            ret += "\n" + util.wrap_text(initial + self._p_return_description
                                         ,begin=begin + " "*settings.INDENT
                                         ,after=" "*len(initial)
                                         ,columns=settings.COLUMNS)
        #
        # Add the end doc string tag to the source code.
        #
        ret += '%s%s"""\n' % (begin," "*settings.INDENT)
        #
        # Return the source code doc string.
        #
        return ret


    def __build_lines_(self, lines, begin):
        """
        Getter method.

        lines : A list of code lines used to generate the returned source code lines.

        begin : The indent or begin string that is appended to every line of returned code.

        return : A string that is the source code lines generated from the given list of code lines
                 and this function's inline comments.
        """
        #
        # If the given lines of code is empty then return a simple pass line.
        #
        if not lines : return begin + " " * settings.INDENT + "pass\n"
        #
        # Initialize the empty source code and inline comments list generated from this function.
        #
        ret = ""
        inlines = [line for line in self._p_inlines.split("\n\n") if line]
        #
        # Iterate through the given code lines.
        #
        for line in lines :
            #
            # Check if this code line is an inline comment marker.
            #
            if line.endswith("#") :
                #
                # If there are still inline comments to add then add the next one in the list to the
                # source code.
                #
                if inlines :
                    ret += begin + " " * settings.INDENT + line + "\n"
                    ret += util.wrap_text(inlines.pop(0)
                                          ,begin=begin + " " * settings.INDENT + line + " "
                                          ,columns=settings.COLUMNS)
                    ret += begin + " " * settings.INDENT + line + "\n"
                #
                # Else there are no more inline comments so add the marker itself to the source
                # code.
                #
                else: ret += begin + " " * settings.INDENT + line + "\n"
            #
            # Else this is a regular code line so add it to the source code.
            #
            else: ret += begin + " " * settings.INDENT + line + "\n"
        #
        # Return the source code lines.
        #
        return ret
