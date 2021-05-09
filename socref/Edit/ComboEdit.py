"""
Contains the ComboEdit class.
"""




class ComboEdit():
    """
    This is the combo edit class. It provides a block property edit definition
    for a combo box edit widget. A combo box edit widget allows selecting a
    value from multiple selection values.
    """


    def __init__(
        self
        ,label
        ,key
        ,selections
    ):
        """
        Initializes this new combo edit definition with the given label, key,
        and selections.

        Parameters
        ----------
        label : string
                The label displayed to the user next to the combo box.
        key : string
              The block property attribute name the combo box modifies.
        selections : list
                     Tuples of valid selection values for the combo box. Each
                     tuple must contain an optional Qt icon and text value in
                     that order. If there is no icon that tuple value can be
                     None.
        """
        self.__label = label
        self.__key = key
        self.__selections = selections


    def key(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : string
                 The block property attribute name the combo box modifies.
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
                 The label displayed to the user next to the combo box.
        """
        return self.__label


    def selections(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : list
                 All valid string selection values for the combo box.
        """
        return self.__selections
