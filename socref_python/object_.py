"""
Contains the object block definition.
"""
import html
from PySide2 import QtGui as qtg
from socref import util
from socref import abstract
from . import settings
from . import package
from . import function
from . import access








@abstract.register_block("Object")
class Block(package.Block):
    """
    This is the object block class. It inherits the python package block. It implements the
    Socrates' Reference abstract block class. It represents a python object.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new object block.
        """
        package.Block.__init__(self)
        #
        # Initialize this block's properties.
        #
        self._p_assignment = ""


    ##########################
    # PUBLIC - Basic Methods #
    ##########################


    def in_class(self):
        """
        Getter method.

        return : True if this object is part of a class or false otherwise.
        """
        return isinstance(self.parent(),access.Block)


    def is_argument(self):
        """
        Getter method.

        return : True if this object is an argument of a function or false otherwise.
        """
        return isinstance(self.parent(),function.Block)


    def icon(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return qtg.QIcon(":/python/object.svg")


    def display_name(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        #
        # Initialize the return string to this block's raw name.
        #
        ret = self._p_name
        #
        # If this object is an argument and has an assignment then add an indicator to the return
        # string.
        #
        if self.is_argument() and self._p_assignment :
            ret += " ="
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
        # Generate the rich text assignment view of this block.
        #
        assignment = html.escape(self._p_assignment)
        if assignment : assignment = "<h2>Assignment</h2><p>%s</p>" % assignment
        #
        # Return the description and assignment view.
        #
        return package.Block.display_view(self) + assignment


    def argument_view(self):
        """
        Getter method.

        return : Rich text paragraph that describes this object as an argument of a function.
        """
        ret = "<p><b>%s " % html.escape(self._p_name)
        if self._p_assignment : ret += " =</b> " + html.escape(self._p_assignment) + " : "
        else: ret += "</b> : "
        ret += html.escape(self._p_description) + "</p>"
        return ret


    def build_list(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return ()


    def is_volatile_above(self):
        """
        Implements the socref.abstract.Block interface.

        return : See interface docs.
        """
        return self.is_argument()


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
        # Add the assignment edit definition.
        #
        ret.append(util.line_edit("Assignment:","_p_assignment"))
        #
        # Return the list.
        #
        return ret


    def set_default_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        package.Block.set_default_properties(self)
        self._p_name = "object"
        self._p_assignment = ""


    def clear_properties(self):
        """
        Implements the socref.abstract.Block interface.
        """
        package.Block.clear_properties(self)
        self._p_assignment = ""


    ############################
    # PUBLIC - Parsing Methods #
    ############################


    def argument(self):
        """
        Getter method.

        return : A string that is the source code for this object as a function argument.
        """
        ret = self._p_name
        if self._p_assignment : ret += "=" + self._p_assignment
        return ret


    def comment(self, begin):
        """
        Detailed description.

        begin : The indent or begin string that is appended to every line of returned source code.

        return : A string that is the source code doc string fragment for this object as a function
                 argument.
        """
        initial = self._p_name + " : "
        return util.wrap_text(initial + self._p_description
                              ,begin=begin
                              ,after=" "*len(initial)
                              ,columns=settings.COLUMNS)


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
        # object is in a class.
        #
        if self.in_class() :
            if previous is None : ret = "\n"*settings.H3LINES
            elif previous._TYPE_ == "Function" : ret = "\n"*settings.H2LINES
            else: ret = ""
        else:
            if previous is None : ret = "\n"*settings.H3LINES
            elif previous._TYPE_ == "Function" : ret = "\n"*settings.H2LINES
            elif previous._TYPE_ == "Class" : ret = "\n"*settings.H1LINES
            else: ret = ""
        #
        # Return the determined number of blank lines, if any.
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
        # Add the description comment to the source code.
        #
        ret = begin + "#\n"
        ret += util.wrap_blocks(self._p_description,begin=begin + "# ",columns=settings.COLUMNS)
        ret += begin + "#\n"
        #
        # Add the object name to the source code, adding its assignment if there is one.
        #
        ret += begin + self._p_name
        if self._p_assignment : ret += " = " + self._p_assignment
        ret += "\n"
        #
        # Return the source code.
        #
        return ret
