"""
Contains the HiddenEdit class.
"""




class HiddenEdit():
    """
    This is the hidden edit class. It provides a block property edit definition
    for a hidden edit widget. This is simply a hidden widget the user cannot see
    that sets a specific block property to a specific value.
    """


    def __init__(
        self
        ,key
        ,value
    ):
        """
        Initializes this new hidden edit definition with the given key and
        value.

        Parameters
        ----------
        key : string
              The block property attribute name the hidden widget modifies.
        value : string
                The value the hidden widget sets to the block property.
        """
        self.__key = key
        self.__value = value


    def key(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : string
                 The block property attribute name the hidden widget modifies.
        """
        return self.__key


    def value(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : string
                 The value the hidden widget sets to the block property.
        """
        return self.__value
