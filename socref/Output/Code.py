"""
Contains the Code class.
"""




class Code():
    """
    This is the code class. It is a container and builder utility class that
    stores lines of code. Methods are provided to add lines of code to a code
    instance that are generic enough any language should be able to use them.
    This is a helper class meant to be used with writer implementations when
    generating their source code output.
    """


    def __init__(
        self
        ,indent
    ):
        """
        Initializes this new code container with the given indent. The indent is
        the string appended to lines of code for each level of depth.

        Parameters
        ----------
        indent : string
                 The indent.
        """
        self.__a = ""
        self.__indent = indent
        self.__lines = []


    def __iter__(
        self
    ):
        return self.__lines.__iter__()


    def add(
        self
        ,lines
    ):
        """
        Adds the given lines or line to this code's lines, adding the currently
        set indent depth to each line given. The given code must be a string if
        it is a single line or a list of strings if it is multiple lines.

        Parameters
        ----------
        lines : object
                The given line or lines of code.
        """
        if isinstance(lines,list):
            self.__lines += [self.__a+line if line else "" for line in lines]
        else:
            self.__lines.append(self.__a+lines if lines else "")


    def addBlank(
        self
        ,amount
    ):
        """
        Adds the given number of blank lines to this code's lines.

        Parameters
        ----------
        amount : int
                 The number of blank lines.
        """
        self.__lines += [""]*amount


    def addText(
        self
        ,text
        ,maxCols
        ,begin=""
        ,after=""
        ,sep=""
    ):
        """
        Adds the given text of lines into this code's lines, wrapping the text
        to limit each line to the maximum columns given. Paragraphs in the given
        text are separated by using two new line characters. Single new line
        characters in the given text are ignored.

        Optional begin, after, and separator strings can also be provided. The
        begin string is added to the beginning of each wrapped line of text and
        paragraph separator lines. The after string is added after the begin
        string in the same way except for very first line. The separator string
        is added to after the begin and after strings to paragraph separator
        lines.

        Parameters
        ----------
        text : string
               The text.
        maxCols : int
                  The maximum number of columns.
        begin : string
                The begin string.
        after : string
                The after string.
        sep : string
              The separator string.
        """
        blocks = [b for b in text.split("\n\n") if b]
        firstW = True
        firstB = True
        for block in blocks:
            if firstB:
                firstB = False
            else:
                self.add(begin+after+sep)
            words = block.split()
            while words:
                if firstW:
                    line = begin+words.pop(0)
                    firstW = False
                else:
                    line = begin+after+words.pop(0)
                while words and (len(self.__a)+len(line)+len(words[0])+1) <= maxCols:
                    line += " "+words.pop(0)
                self.add(line)


    def setDepth(
        self
        ,depth
    ):
        """
        Sets this code's indent depth to the given value. Any given value less
        than zero will be treated as zero.

        Parameters
        ----------
        depth : int
                The indent depth.
        """
        self.__a = self.__indent*depth
