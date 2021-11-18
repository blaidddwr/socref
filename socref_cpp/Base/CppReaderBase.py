"""
Contains the CppReaderBase class.
"""
from re import compile as reCompile
from socref.Base.ReaderBase import ReaderBase




class CppReaderBase(ReaderBase):
    """
    This is the C++ reader base. It partially implements the Socrates' Reference
    abstract reader class. It provides common methods used by multiple other C++
    reader classes.
    """
    __scopeRE = reCompile("^namespace +([a-zA-Z_]+\w*) +{$")
    __namespaceRE = reCompile("^namespace +([a-zA-Z_]+\w*)$")


    def scope(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : list
                 Names of namespaces where this reader resides, starting from
                 the global namespace and ending last one. This includes this
                 reader if it is a namespace or class reader.
        """
        return []


    def _scanHeadBody_(
        self
    ):
        """
        Scans for any namespace, class, enumeration, or union declarations. New
        namespace, class, enumeration, or union readers are created as children
        of this reader for each declaration found.
        """
        NamespaceReader = GNamespaceReader.NamespaceReader
        d = 0
        while True:
            self.save()
            (i,line) = self.read()
            if line is None:
                return
            match = self.__namespaceRE.match(line)
            if match:
                name = match.group(1)
                reader = NamespaceReader(name,self)
                reader()
                continue
            if line:
                d += line.count("{")-line.count("}")
                if d < 0:
                    self.restore()
                    return
            self.discard()


    def _scanHeader_(
        self
    ):
        """
        Scans for header lines of code and returns them, removing indentation
        spaces from each line where the indentation is determined from the first
        line scanned. This stops scanning lines once it encounters an empty line
        or a final closing bracket with no previous opening bracket. If the
        special end macro is encountered as the last line it is also discarded.

        Returns
        -------
        result : list
                 The header lines of code.
        """
        ret = []
        d = 0
        ind = None
        while True:
            (i,line) = self.read()
            if not line:
                break
            else:
                d += line.count("{")-line.count("}")
                if d < 0:
                    break
                if ind is None:
                    ind = i
                ret.append(" "*max(0,i-ind) + line)
        if ret and ret[-1] == "#endif":
            ret.pop(-1)
        return ret


    def _scanScope_(
        self
    ):
        """
        Scans for namespace scope declarations and returns them as a list of
        scoped namespace names. This stops scanning if any line that does not
        match a scoping namespace declaration is encountered.

        Returns
        -------
        result : list
                 The list of scoped namespace names.
        """
        ret = []
        while True:
            self.save()
            (i,line) = self.read()
            if not line:
                self.discard()
                break
            match = self.__scopeRE.match(line)
            if match:
                ret.append(match.group(1))
            else:
                self.restore()
                break
            self.discard()
        return ret


    def _skipBlanks_(
        self
    ):
        """
        Skips blank lines until the end of the file is reached or a line that is
        not empty.
        """
        while True:
            self.save()
            (i,line) = self.read()
            if line is None:
                return
            if line:
                self.restore()
                return
            self.discard()


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


from ..Reader import NamespaceReader as GNamespaceReader
