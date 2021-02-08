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
        Initializes this new code container with the given indent.

        Parameters
        ----------
        indent : string
                 The indent that equals one level or depth of code.
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
        set indent depth to each line given.

        Parameters
        ----------
        lines : object
                A line or lines of code that are added to this code's lines with
                this code's current indent depth. This must be a string or list
                of strings.
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
                 The number of blank lines added to this code's lines.
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
        to limit each line to the maximum columns given. An optional begin
        string is given that is added to the beginning of each generated line of
        text. An optional after string is given that is added to the beginning
        of each generated line of text except for the very first line. An
        optional separator string is given that is used to separate multiple
        paragraphs of text as a separate line.

        Parameters
        ----------
        text : string
               The text that is wrapped into lines and added to this code's
               lines. Paragraphs are separated by using two new line characters.
               Single new line characters are ignored.
        maxCols : int
                  The maximum number of columns for each line of wrapped text.
        begin : string
                The string that is added to the beginning of each wrapped line
                of text and the special paragraph separator lines.
        after : string
                The string that is added to the beginning of each wrapped line
                of text and the special paragraph separator lines, excluding the
                very first line.
        sep : string
              The string that is used as a line separator between paragraphs of
              text.
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
        Sets this code's indent depth to the given value.

        Parameters
        ----------
        depth : int
                The new indent depth of this code. Anything less than zero will
                be treated as zero.
        """
        self.__a = self.__indent*depth
