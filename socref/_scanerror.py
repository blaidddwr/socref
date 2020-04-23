"""
Contains the ScanError class.
"""








class ScanError(Exception):
    """
    This is the scan error exception class. It indicates an error occurred while scanning source
    files while parsing a project.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(
        self
        ,*args
        ):
        """
        Initialize a new load error exception with any number of given positional arguments.

        Parameters
        ----------
        *args : tuple
                Positional arguments passed on to the super exception class.
        """
        Exception.__init__(self,*args)
