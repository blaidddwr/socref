"""
Contains the LoadError class.
"""








class LoadError(Exception):
    """
    This is the load error exception class. It indicates an error occurred
    attempting to load a project from its saved file.
    """


    def __init__(
        self
        ,*args
        ):
        """
        Initialize a new load error exception with any number of given
        positional arguments.

        Parameters
        ----------
        *args : tuple
                Positional arguments passed on to the super exception class.
        """
        Exception.__init__(self,*args)
