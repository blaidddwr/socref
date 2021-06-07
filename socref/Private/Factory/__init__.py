"""
Contains all factory classes and their singleton instances.
"""
from .BlockFactory import BlockFactory
from .ParserFactory import ParserFactory


blockFactory = BlockFactory()
parserFactory = ParserFactory()
