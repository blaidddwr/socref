"""
Contains the TextEdit class.
"""




class TextEdit():
    """
    Detailed description.
    """


    def __init__(
        self
        ,label
        ,key
        ,spellCheck=False
    ):
        """
        Detailed description.

        Parameters
        ----------
        label : 
        key : 
        spellCheck : 
        """
        self.__label = label
        self.__key = key
        self.__spellCheck = spellCheck


    def key(
        self
    ):
        """
        Detailed description.
        """
        return self.__key


    def label(
        self
    ):
        """
        Detailed description.
        """
        return self.__label


    def spellCheck(
        self
    ):
        """
        Detailed description.
        """
        return self.__spellCheck
