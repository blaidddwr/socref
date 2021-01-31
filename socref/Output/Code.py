"""
Contains the Code class.
"""




class Code():
    """
    Detailed description.
    """


    def __init__(
        self
        ,indent
    ):
        """
        Detailed description.

        Parameters
        ----------
        indent : object
                 Detailed description.
        """
        self.__a = ""
        self.__andent = indent
        self.__lines = []


    def __iter__(
        self
    ):
        """
        Detailed description.
        """
        return self.__lines.__iter__()


    def add(
        self
        ,lines
    ):
        """
        Detailed description.

        Parameters
        ----------
        lines : object
                Detailed description.
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
        Detailed description.

        Parameters
        ----------
        amount : object
                 Detailed description.
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
        Detailed description.

        Parameters
        ----------
        text : object
               Detailed description.
        maxCols : object
                  Detailed description.
        begin : object
                Detailed description.
        after : object
                Detailed description.
        sep : object
              Detailed description.
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
                while words and (len(self.__a)+len(line)+len(words[0])) <= maxCols:
                    line += " "+words.pop(0)
                self.add(line)


    def setDepth(
        self
        ,depth
    ):
        """
        Detailed description.

        Parameters
        ----------
        depth : object
                Detailed description.
        """
        self.__a = self.__andent*depth
