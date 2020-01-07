"""
Contains all custom exception classes that the core application can raise.
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

        *args : Positional arguments passed on to the super exception class.
        """
        Exception.__init__(self,*args)








class LoadError(Exception):
    """
    This is the load error exception class. It indicates an error occurred attempting to load a
    project from its saved file.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, *args):
        """
        Initialize a new load error exception with any number of given positional arguments.

        *args : Positional arguments passed on to the super exception class.
        """
        Exception.__init__(self,*args)








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

        *args : Positional arguments passed on to the super exception class.
        """
        Exception.__init__(self,*args)








class ScanError(Exception):
    """
    This is the scan error exception class. It indicates an error occurred while scanning source
    files while parsing a project.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, *args):
        """
        Initialize a new load error exception with any number of given positional arguments.

        *args : Positional arguments passed on to the super exception class.
        """
        Exception.__init__(self,*args)
