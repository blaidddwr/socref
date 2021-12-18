"""
Contains the FileReaderBase class.
"""
from ..Settings import Settings
from .CppReaderBase import CppReaderBase
from re import compile as reCompile




class FileReaderBase(CppReaderBase):
    """
    This is the file reader base class. It implements the Socrates' Reference
    abstract reader class. It parses header or source files; saving macros and
    header lines.
    """
    __scopeRE = reCompile("^namespace +([a-zA-Z_]+\w*) +{$")


    def __init__(
        self
        ,parent
    ):
        """
        Initializes this new file reader base with the given parent parser.

        Parameters
        ----------
        parent : Parser
                 The parent parser.
        """
        super().__init__(parent)
        self.__macros = []
        self.__header = []
        self.__scope = []


    def header(
        self
    ):
        """
        Returns header lines of code parsed after all macros and name space
        scope declarations of this reader's file. This can only be called once,
        after which an empty list is returned.

        Returns
        -------
        result : list
                 The header lines of code or an empty list.
        """
        ret = self.__header
        self.__header = []
        return ret


    def macros(
        self
    ):
        """
        Returns macro lines of code parsed after the header guard macro lines of
        this reader's file. This can only be called once, after which an empty
        list is returned.

        Returns
        -------
        result : list
                 The macro lines of code or an empty list.
        """
        ret = self.__macros
        self.__macros = []
        return ret


    def scope(
        self
    ):
        return self.__scope


    def unknown(
        self
    ):
        ret = []
        if self.__macros:
            ret += ["INCLUDES:"]+self.__macros
        if self.__header:
            ret += ["HEADER:"]+self.__header
        return ret


    def _scan_(
        self
    ):
        self.__macros = self.__scanMacros_()
        self.__scope = self.__scanScope_()
        self.__header = self._scanHeader_()


    def _skipMacros_(
        self
        ,amount
    ):
        """
        Skips the given number of macro lines. Lines that are not macro lines
        are ignored and also skipped until the given number of macro lines are
        skipped.

        Parameters
        ----------
        amount : int
                 The number of macro lines.
        """
        m = 0
        while True:
            self.save()
            (i,line) = self.read()
            if not line or not line.startswith("#"):
                self.restore()
                return
            m += 1
            if m >= amount:
                self.discard()
                return


    def __scanMacros_(
        self
    ):
        """
        Scans for macro lines of code and returns them. This stops scanning when
        the first non macro line is encountered.

        Returns
        -------
        result : list
                 The macro lines of code.
        """
        ret = []
        while True:
            self.save()
            (i,line) = self.read()
            if line is None:
                break
            elif line.startswith("#"):
                self.discard()
                ret.append(" "*i + line)
            else:
                self.restore()
                break
        return ret


    def __scanScope_(
        self
    ):
        """
        Scans for name space scope declarations and returns them as a list of
        space names. This stops scanning on the first non scoping name space
        declaration line encountered.

        Returns
        -------
        result : list
                 The list of scoped name spaces.
        """
        ret = []
        while True:
            self.save()
            (i,line) = self.read()
            if not line:
                self.restore()
                break
            match = self.__scopeRE.match(line)
            if match:
                ret.append(match.group(1))
            else:
                self.restore()
                break
            self.discard()
        return ret
