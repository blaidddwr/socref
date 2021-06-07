"""
Contains all controller classes and their singleton instances.
"""
from .LangController import LangController
from .ParseController import ParseController


langController = LangController()
parseController = ParseController()
