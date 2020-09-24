"""
Contains all core classes, functions, and singleton instances of this
application.
"""
import enum
import os
import hunspell
from PySide2 import QtCore as qtc
from . import settings

from ._blockfactory import BlockFactory
from ._insertcommand import InsertCommand
from ._movecommand import MoveCommand
from ._parsermodel import ParserModel
from ._projectmodel import ProjectModel
from ._removecommand import RemoveCommand
from ._setcommand import SetCommand








class Role(enum.IntEnum):
    """
    This enumerates all custom data roles the project model implements.
    """
    BlockType = qtc.Qt.UserRole+0
    View = qtc.Qt.UserRole+1
    BuildList = qtc.Qt.UserRole+2
    Properties = qtc.Qt.UserRole+3
    EditDefs = qtc.Qt.UserRole+4




def initialize():
    """
    Initializes the global singleton Hunspell speller used for spell checking
    through this application. This must only be called once during
    initialization of the application.
    """
    global speller
    speller = hunspell.HunSpell(
        os.path.join(settings.HUNSPELL_ROOT,settings.DICTIONARY+".dic")
        ,os.path.join(settings.HUNSPELL_ROOT,settings.DICTIONARY+".aff")
    )


#
# The global singleton instance of this application's block factory.
#
blockFactory = BlockFactory()


#
# The global singleton instance of this application's Hunspell speller.
#
speller = None


#
# The global singleton instance of this application's parser model.
#
parser = ParserModel()
