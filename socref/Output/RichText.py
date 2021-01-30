"""
Contains the RichText class.
"""
from html import escape




class RichText():
    """
    Detailed description.
    """


    def __init__(
        self
    ):
        """
        Detailed description.
        """
        self.__frags = []


    def __str__(
        self
    ):
        """
        Detailed description.
        """
        return "".join(self.__frags)


    def addHeader(
        self
        ,level
        ,title
    ):
        """
        Detailed description.

        Parameters
        ----------
        level : object
                Detailed description.
        title : object
                Detailed description.
        """
        level = min(3,max(1,level))
        self.__frags.append("<h%i>%s</h%i>"%(level,escape(title),level))


    def addList(
        self
        ,items
    ):
        """
        Detailed description.

        Parameters
        ----------
        items : object
                Detailed description.
        """
        self.__frags.append("<p>")
        for item in items:
            self.__frags += [escape(item),"<br/>"]
        self.__frags.append("</p>")


    def addMap(
        self
        ,items
    ):
        """
        Detailed description.

        Parameters
        ----------
        items : object
                Detailed description.
        """
        self.__frags.append("<p>")
        for (header,text) in items:
            self.__frags += ["<b>",escape(header),"</b>",escape(text),"<br/>"]
        self.__frags.append("</p>")


    def addText(
        self
        ,text
    ):
        """
        Detailed description.

        Parameters
        ----------
        text : object
               Detailed description.
        """
        self.__frags.append("<p>")
        for sub in text.split("\n\n"):
            if sub:
                self.__frags += ["</p><p>",escape(sub)]
        self.__frags.append("</p>")
