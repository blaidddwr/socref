"""
Contains the Scanner class.
"""








class Scanner():
    """
    This is the parser scanner class. It provides an object oriented scanner for an input file given
    to it at initialization. This scans python lines of code, skipping blank lines. It keeps track
    of the block of code it is scanning by the indent, stopping once a smaller indent is
    encountered.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(
        self
        ,ifile
        ):
        """
        Initializes a new scanner with the given input file.

        Parameters
        ----------
        ifile : file object
                The input file that is scanned.
        """
        self.__ifile = ifile
        self.__indent = None


    ####################
    # PUBLIC - Methods #
    ####################


    def indent(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : int
               The indent size in spaces of the code block this scanner is reading. This must be
               called after the read line method is called at least once.
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
               The next line of python source code read in from this scanner's input file. If the
               end of file is reached or the end of the code block with the determined indent then
               this returns none. The first time this is called the indent of the block to be read
               is determined with the first line of code encountered.
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
