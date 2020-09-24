"""
Contains the Scanner class.
"""








class Scanner():
    """
    This is the parser scanner class. It provides an object oriented scanner for
    an input file given to it at initialization. This scans python lines of
    code, skipping blank lines. It keeps track of the block of code it is
    scanning by the indent, stopping once a smaller indent is encountered.
    """


    def __init__(
        self
        ,ifile
        ,indent=None
        ):
        """
        Initializes a new scanner with the given input file.

        Parameters
        ----------
        ifile : io.TextIOWrapper
                The input file that is scanned.
        indent : object
                 The indent size for the block of code scanned or none to
                 automatically determine it with the indent of the first line of
                 code scanned. The default is none.
        """
        self.__ifile = ifile
        self.__indent = indent


    def indent(
        self
        ):
        """
        Getter method. This must be called after the indent is known.

        Returns
        -------
        ret0 : int
               The indent size in spaces of the code block this scanner is
               reading.
        """
        if self.__indent is None:
            raise RuntimeError("indent has not been discovered.")
        return self.__indent


    def readline(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               The next line of python source code read in from this scanner's
               input file. If the end of file is reached or the end of the code
               block with the determined indent then this returns none. The
               first time this is called the indent of the block to be read is
               determined with the first line of code encountered.
        """
        while True:
            last = self.__ifile.tell()
            line = self.__ifile.readline()
            if not line:
                return None
            if line != '\n':
                line = line[:-1]
                indent = len(line) - len(line.lstrip(' '))
                if self.__indent is None:
                    self.__indent = indent
                elif indent < self.__indent:
                    self.__ifile.seek(last)
                    return None
                return line
