"""
Contains the TextEdit class.
"""




class TextEdit():
    """
    This is the text edit class. It provides a block property edit definition
    for a text edit widget. A text edit widget is capable of editing multiple
    lines of text and provide optional spell checking.
    """


    def __init__(
        self
        ,label
        ,key
        ,spellCheck=False
    ):
        """
        Initializes this new text edit definition with the given label, key, and
        optional spell checking flag. The given spelling checking flag is true
        to enable spell checking else false to disable it.

        Parameters
        ----------
        label : string
                The label.
        key : string
              The key.
        spellCheck : bool
                     The spell checking flag.
        """
        self.__label = label
        self.__key = key
        self.__spellCheck = spellCheck


    def key(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : string
                 The block property attribute name the text edit widget
                 modifies.
        """
        return self.__key


    def label(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : string
                 The label displayed to the user next to the text edit widget.
        """
        return self.__label


    def spellCheck(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True to enable spell checking for the text edit widget or false
                 to disable it.
        """
        return self.__spellCheck
