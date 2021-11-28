"""
Contains the RichText class.
"""
from html import escape




class RichText():
    """
    This is the rich text class. It is a container and builder utility class
    that stores Qt rich text for display. Methods are provided to add different
    types of rich text to a rich text instance. This is a helper class meant to
    be used with a block's display view interface.
    """


    def __init__(
        self
    ):
        self.__frags = []


    def __str__(
        self
    ):
        return "".join(self.__frags)


    def addBox(
        self
        ,title
        ,text
    ):
        """
        Adds a box style paragraph to this rich text with the given text and
        title. A box paragraph is split into two parts. The first part is the
        title which is bold and alone on the first line. The second part is the
        normal text which begins on the second line.

        Parameters
        ----------
        title : string
                The title.
        text : string
               The text.
        """
        self.__frags += ["<p><b>",escape(title),"</b><br/>",escape(text),"</p>"]


    def addHeader(
        self
        ,title
        ,level
    ):
        """
        Adds a header to this rich text with the given title and level. The
        allowed levels are 1, 2, or 3.

        Parameters
        ----------
        title : string
                The title.
        level : int
                The header level.
        """
        level = min(3,max(1,level))
        self.__frags.append("<h%i>%s</h%i>"%(level,escape(title),level))


    def addList(
        self
        ,items
    ):
        """
        Adds an unordered list to this rich text from the given list of items.
        The given list of items must be a list of strings.

        Parameters
        ----------
        items : list
                The list of items.
        """
        self.__frags.append("<p>")
        first = True
        for item in items:
            if first:
                first = False
            else:
                self.__frags.append("<br/>")
            self.__frags.append(escape(item))
        self.__frags.append("</p>")


    def addMap(
        self
        ,items
    ):
        """
        Adds a map list to this rich text from the given list of items. A map
        list is a paragraph where each item is listed on a new line within the
        paragraph. The given list of items must be a list of strings.

        Parameters
        ----------
        items : list
                The list of items.
        """
        self.__frags.append("<p>")
        first = True
        for (header,text) in items:
            if first:
                first = False
            else:
                self.__frags.append("<br/>")
            self.__frags += ["<b>",escape(header),"</b>",escape(text)]
        self.__frags.append("</p>")


    def addText(
        self
        ,text
    ):
        """
        Adds a paragraph to this rich text with the given text.

        Parameters
        ----------
        text : string
               The text.
        """
        self.__frags.append("<p>")
        for sub in text.split("\n\n"):
            if sub:
                self.__frags += ["</p><p>",escape(sub)]
        self.__frags.append("</p>")
