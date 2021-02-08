"""
Contains the CheckBoxEdit class.
"""




class CheckBoxEdit():
    """
    This is the checkbox edit class. It provides a block property edit
    definition for a checkbox edit widget. A checkbox edit widget allows
    selecting true or false, where the string "1" is true and "0" is false.
    """


    def __init__(
        self
        ,label
        ,key
    ):
        """
        Initializes this new checkbox edit definition with the given label and
        key.

        Parameters
        ----------
        label : string
                The label displayed to the user next to the checkbox.
        key : string
              The block property attribute name the checkbox modifies.
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
                 The block property attribute name the checkbox modifies.
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
                 The label displayed to the user next to the checkbox.
        """
        return self.__label
