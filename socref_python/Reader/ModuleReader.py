"""
Contains the ModuleReader class.
"""
from re import compile as reCompile
from socref.Abstract.AbstractReader import *




class ModuleReader(AbstractReader):
    """
    Detailed description.
    """
    __preHeaderRE = reCompile('^ *#.*')
    __headerRE = reCompile('^(from|import).*')
    __functionRE = reCompile('^def +([a-zA-Z_]+\w*)\((.*)')
    __classRE = reCompile('^class +([a-zA-Z_]+\w*)\([\w\._,\s]*\):')


    def __init__(
        self
        ,block
        ,parent
    ):
        """
        asdf

        Parameters
        ----------
        block : object
                Detailed description.
        parent : object
                 Detailed description.
        """
        super().__init__(parent)
        self._setKey_(block.key())
        self.__preHeader = []
        self.__header = []
        self.__footer = []


    def footer(
        self
    ):
        """
        Detailed description.
        """
        ret = self.__footer
        self.__footer = []
        return ret


    def header(
        self
    ):
        """
        Detailed description.
        """
        ret = self.__header
        self.__header = []
        return ret


    def preHeader(
        self
    ):
        """
        Detailed description.
        """
        ret = self.__preHeader
        self.__preHeader = []
        return ret


    def unknown(
        self
    ):
        """
        Detailed description.
        """
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
        """
        Detailed description.
        """
        self.__scanPreHeader_()
        self.__skipDocString_()
        self.__scanHeader_()
        self.__scanBody_()
        self.__scanFooter_()


    def __scanBody_(
        self
    ):
        """
        Scans the body of code looking for functions or classes, ending at the
        first nonempty line that is not a definition of either one or end of
        file.
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
                # function
                continue
            match = self.__classRE.match(line)
            if match:
                name = match.group(1)
                # class
                continue
            if line:
                self.restore()
                return
            self.discard()


    def __scanFooter_(
        self
    ):
        """
        Scans script code lines from the end.
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
        Scans header import lines.
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
        Scans initial comment lines from the very beginning.
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


    def __skipDocString_(
        self
    ):
        """
        Skips any documentation string. Blank lines also skipped and ignored.
        """
        count = 0
        while count < 2:
            self.save()
            (i,line) = self.read()
            if line is None:
                return
            if line == '"""':
                count += 1
            elif line and count == 0:
                self.restore()
                return
            self.discard()
