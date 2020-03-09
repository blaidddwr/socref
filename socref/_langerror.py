"""
Contains the LangError class.
"""








class LangError(Exception):
    """
    This is the import error exception class. It indicates an error occurred while loading a
    language's module.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, *args):
        """
        Initialize a new load error exception with any number of given positional arguments.

        Parameters
        ----------
        *args : tuple
                Positional arguments passed on to the super exception class.
        """
        Exception.__init__(self,*args)
