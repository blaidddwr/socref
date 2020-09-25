"""
Contains all custom exception classes that this application can raise.
"""








class LangError(Exception):
    """
    This is the import error exception class. It indicates an error occurred
    while loading a language's module.
    """
    pass








class LoadError(Exception):
    """
    This is the load error exception class. It indicates an error occurred
    attempting to load a project from its saved file.
    """
    pass








class RegisterError(Exception):
    """
    This is the register error exception class. It indicates an error occurred
    attempting to register a block while loading a language's module.
    """
    pass








class ScanError(Exception):
    """
    This is the scan error exception class. It indicates an error occurred while
    scanning source files while parsing a project.
    """
    pass
