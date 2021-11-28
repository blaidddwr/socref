"""
Contains the Parser class.
"""
from socref import parser
from socref_cpp.Parser import Parser as CppParser




@parser
class Parser(CppParser):
    """
    This is the parser class. It implements the Socrates' Reference abstract
    parser class for the C++/Qt language.
    """
    pass
