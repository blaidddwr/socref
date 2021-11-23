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
    __classRE = reCompile("^(public: )?class +([a-zA-Z_]+\w*)$")
    __enumerationRE = reCompile("^(public: )?enum +class +([a-zA-Z_]+\w*)$")
    __unionRE = reCompile("^(public: )?union +([a-zA-Z_]+\w*)$")


    def scope(
        self
    ):
        """
        This interface is a getter method.

        Returns
        -------
        result : list
                 Names of name spaces of this reader's scope, starting from the
                 global name space and ending in the last scoped. This includes
                 any class name spaces.
        """
        return []


    def _scanDeclarations_(
        self
    ):
        """
        Scans for any class, enumeration, or union declarations. New class,
        enumeration, or union readers are created as children of this reader for
        each declaration found.
        """
        ClassReader = GClassReader.ClassReader
        EnumerationReader = GEnumerationReader.EnumerationReader
        UnionReader = GUnionReader.UnionReader
        d = 0
        while True:
            self.save()
            (i,line) = self.read()
            if line is None:
                return
            match = self.__classRE.match(line)
            if match:
                name = match.group(2)
                reader = ClassReader(name,self)
                reader()
                continue
            match = self.__enumerationRE.match(line)
            if match:
                name = match.group(2)
                reader = EnumerationReader(name,self)
                reader()
                continue
            match = self.__unionRE.match(line)
            if match:
                name = match.group(2)
                reader = UnionReader(name,self)
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


from ..Reader import ClassReader as GClassReader
from ..Reader import EnumerationReader as GEnumerationReader
from ..Reader import UnionReader as GUnionReader
