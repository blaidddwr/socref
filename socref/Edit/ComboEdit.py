"""
Contains the ComboEdit class.
"""




class ComboEdit():
    """
    Detailed description.
    """


    def __init__(
        self
        ,label
        ,key
        ,selections
    ):
        """
        Detailed description.

        Parameters
        ----------
        label : object
                Detailed description.
        key : object
              Detailed description.
        selections : object
                     Detailed description.
        """
        self.__label = label
        self.__key = key
        self.__selections = selections


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


    def selections(
        self
    ):
        """
        Detailed description.
        """
        return self.__selections
