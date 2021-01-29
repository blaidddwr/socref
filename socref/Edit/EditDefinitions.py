"""
Contains the EditDefinitions class.
"""




class EditDefinitions():
    """
    Detailed description.
    """


    def __init__(
        self
    ):
        """
        Detailed description.
        """
        self.__edits = []


    def __iter__(
        self
    ):
        """
        Detailed description.
        """
        return self.__edits.__iter__()


    def append(
        self
        ,edit
    ):
        """
        Detailed description.

        Parameters
        ----------
        edit : object
               Detailed description.
        """
        self.__edits.append(edit)
