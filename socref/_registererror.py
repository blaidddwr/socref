"""
Contains the RegisterError class.
"""








class RegisterError(Exception):
    """
    This is the register error exception class. It indicates an error occurred attempting to
    register a block while loading a language's module.
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
