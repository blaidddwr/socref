"""
Contains the LineEdit class.
"""




class LineEdit():
    """
    This is the line edit class. It provides a block property edit definition
    for a line edit widget. A line edit widget is capable of editing a single
    line of text.
    """


    def __init__(
        self
        ,label
        ,key
    ):
        """
        Initializes this new line edit definition with the given label and key.

        Parameters
        ----------
        label : string
                The label.
        key : string
              The key.
        """
        self.__label = label
        self.__key = key


    def key(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : string
                 The block property attribute name the line edit widget
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
                 The label displayed to the user next to the line edit widget.
        """
        return self.__label
