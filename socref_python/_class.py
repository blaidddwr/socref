"""
Contains the Class class.
"""
from PySide2 import QtGui as qtg
from socref import public as scr
from ._descriptor import Descriptor
from . import block
from . import settings








@scr.register("Class")
class Class(Descriptor):
    """
    This is the class block class. It implements the Socrates' Reference
    abstract block class. It represents a python class.
    """


    def __init__(
        self
        ):
        """
        Initializes a new class block.
        """
        super().__init__()
        self._p_parents = ""
        self._p_infile = "0"


    def build(
        self
        ,definition
        ,begin=""
        ):
        """
        Implements the socref_python.block.Package interface.

        Parameters
        ----------
        definition : object
                     See interface docs.
        begin : object
                See interface docs.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = []
        header = []
        footer = []
        if not self.isInfile():
            (header,footer) = self._build_(
                definition
                ,begin
                ,"Contains the %s class."%(self._p_name,)
            )
            ret += header
        definition = definition["classes"].get(self._p_name,{"functions": {}})
        ret += [""]*settings.H1LINES
        ret += self._buildDescriptors_(begin)
        ret.append("%sclass %s(%s):" % (begin,self._p_name,self.__buildParents_()))
        newBegin = begin + " "*settings.INDENT
        docString = newBegin+'"""'
        ret.append(docString)
        ret += scr.wrapBlocks(self._p_description,newBegin,columns=settings.COLUMNS)
        ret.append(docString)
        (regular,classes) = self._buildChildren_(definition,newBegin)
        body = definition.pop("lines",[]) + regular
        if body:
            ret += body
        else:
            ret += [newBegin+"pass"]
        if not self.isInfile():
            ret += footer
        return ret


    def buildList(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return ("Function","Object","Access")


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Descriptor.clearProperties(self)
        self._p_parents = ""
        self._p_infile = "0"


    def displayName(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return self._p_name + self._descriptorsName_()


    def displayView(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        parents = scr.richText(
            2
            ,"Parents"
            ,"".join(("<li>%s</li>" % parent for parent in self._p_parents.split("\n") if parent))
        )
        flags = scr.richTextList(2,"Flags",["Infile"] if self.isInfile() else [])
        return block.Package.displayView(self) + parents + self._descriptorsView_() + flags


    def editDefinitions(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = block.Package.editDefinitions(self)
        ret.append(scr.textEdit("Parents:","_p_parents"))
        ret.append(scr.checkboxEdit("Infile","_p_infile"))
        ret.append(self._descriptorsEditDefinition_())
        return ret


    def icon(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        if self.isAbstract():
            return qtg.QIcon(":/python/abstract_class.svg")
        else:
            return qtg.QIcon(":/python/class.svg")


    def isAbstract(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this class contains any abstract functions or false
               otherwise.
        """
        for block in self:
            if block._TYPE_ == "Function" and block.isAbstract():
                return True
        return False


    def isInfile(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this class is in file or false otherwise. In file means
               its definition is included in the module it is part of, otherwise
               it is defined in its own special container module and imported
               into its module namespace.
        """
        return bool(int(self._p_infile))


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Descriptor.setDefaultProperties(self)
        self._p_name = "class"
        self._p_parents = ""
        self._p_infile = "0"


    def __buildParents_(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               The source code fragment of this class block's parents used in
               its source code header line.
        """
        return ", ".join((parent for parent in self._p_parents.split("\n") if parent))
