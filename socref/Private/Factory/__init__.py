"""
Contains all factory classes and their singleton instances.
"""
from .BlockFactory import *
from .ParserFactory import *


blockFactory = BlockFactory()
parserFactory = ParserFactory()
