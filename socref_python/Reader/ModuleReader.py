"""
Contains the ModuleReader class.
"""
from . import skipDocString
from .ClassReader import *
from .FunctionReader import *
from re import compile as reCompile
from socref.Base.ReaderBase import *




class ModuleReader(ReaderBase):
    """
    This is the module reader class. It implements the Socrates' Reference
    abstract reader class. It parses module files; saving import lines, function
    definitions, class definitions, and custom footer lines of code. Parsed
    function and class definitions are added as new reader children. This is an
    all purpose reader that reads in any module file, including the special
    package init file and dummy class container modules.
    """
    __preHeaderRE = reCompile("^ *#.*")
    __headerRE = reCompile("^(from|import).*")
    __functionRE = reCompile("^def +([a-zA-Z_]+\w*)\((.*)")
    __classRE = reCompile("^class +([a-zA-Z_]+\w*)\([\w\._,\s]*\):")


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        Initializes this new module reader with the given block and parent
        parser.

        Parameters
        ----------
        block : AbstractBlock
                A package, module, or class block that is associated with this
                reader's parsed python file.
        parent : Parser
                 The parser currently parsing its project's source code files.
        """
        super().__init__(parent)
        self._setKey_(block.key(True))
        self.__preHeader = []
        self.__header = []
        self.__footer = []


    def footer(
        self
    ):
        """
        Getter method. This can only be called once, after which an empty list
        is returned.

        Returns
        -------
        result : list
                 Lines of custom code parsed at the end of this reader's module
                 after all function and class definitions.
        """
        ret = self.__footer
        self.__footer = []
        return ret


    def header(
        self
    ):
        """
        Getter method. This can only be called once, after which an empty list
        is returned.

        Returns
        -------
        result : list
                 Import lines of code parsed after the doc string of this
                 reader's module.
        """
        ret = self.__header
        self.__header = []
        return ret


    def preHeader(
        self
    ):
        """
        Getter method. This can only be called once, after which an empty list
        is returned.

        Returns
        -------
        result : list
                 Special comment lines parsed from the very beginning of this
                 reader's module. The initial pound character is included in
                 each line.
        """
        ret = self.__preHeader
        self.__preHeader = []
        return ret


    def unknown(
        self
    ):
        ret = []
        if self.__preHeader:
            ret += ["PREHEADER:"]+self.__preHeader
        if self.__preHeader:
            ret += ["HEADER:"]+self.__header
        if self.__preHeader:
            ret += ["FOOTER:"]+self.__footer
        return ret


    def _scan_(
        self
    ):
        self.__scanPreHeader_()
        skipDocString(self)
        self.__scanHeader_()
        self.__scanBody_()
        self.__scanFooter_()


    def __scanBody_(
        self
    ):
        """
        Scans for any function or class definitions within this reader's parsed
        module. New function or class readers are created as children of this
        reader for each definition found.
        """
        while True:
            self.save()
            (i,line) = self.read()
            if line is None:
                return
            match = self.__functionRE.match(line)
            if match:
                name = match.group(1)
                end = match.group(2)
                reader = FunctionReader(name,end.endswith("):"),self)
                reader()
                continue
            match = self.__classRE.match(line)
            if match:
                name = match.group(1)
                reader = ClassReader(name,self)
                reader()
                continue
            if line and not line.startswith("@"):
                self.restore()
                return
            self.discard()


    def __scanFooter_(
        self
    ):
        """
        Scans for any custom code lines present after all function and class
        definitions in this reader's parsed module.
        """
        while True:
            (i,line) = self.read()
            if line is None:
                return
            self.__footer.append(" "*i + line)


    def __scanHeader_(
        self
    ):
        """
        Scans for any import code lines present at the beginning of this
        reader's parsed module, positioned right after the module's doc string.
        Scanning stops once the first blank line or non import code line is
        encountered.
        """
        while True:
            self.save()
            (i,line) = self.read()
            if line is None:
                return
            if self.__headerRE.match(line):
                self.__header.append(line)
            else:
                self.restore()
                return
            self.discard()


    def __scanPreHeader_(
        self
    ):
        """
        Scans any special comment lines at the very beginning of this reader's
        parsed module.
        """
        while True:
            self.save()
            (i,line) = self.read()
            if line is None:
                return
            if not self.__preHeaderRE.match(line):
                self.restore()
                return
            self.__preHeader.append(line)
            self.discard()
