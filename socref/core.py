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








blockFactory = BlockFactory()
speller = hunspell.HunSpell(
    os.path.join(settings.HUNSPELL_ROOT,settings.DICTIONARY+".dic")
    ,os.path.join(settings.HUNSPELL_ROOT,settings.DICTIONARY+".aff")
)
parser = ParserModel()
