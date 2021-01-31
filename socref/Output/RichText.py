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


    def addBox(
        self
        ,text
        ,title
    ):
        """
        Detailed description.

        Parameters
        ----------
        text : 
        title : 
        """
        self.__frags += ["<p><b>",escape(title),"</b><br/>",escape(text),"</p>"]


    def addHeader(
        self
        ,level
        ,title
    ):
        """
        Detailed description.

        Parameters
        ----------
        level : 
        title : 
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
        items : 
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
        Detailed description.

        Parameters
        ----------
        items : 
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
        Detailed description.

        Parameters
        ----------
        text : 
        """
        self.__frags.append("<p>")
        for sub in text.split("\n\n"):
            if sub:
                self.__frags += ["</p><p>",escape(sub)]
        self.__frags.append("</p>")
