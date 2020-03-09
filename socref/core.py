"""
asd
"""
import os
import hunspell
from . import settings

from ._blockfactory import BlockFactory
from ._insertcommand import InsertCommand
from ._movecommand import MoveCommand
from ._parsermodel import ParserModel
from ._projectmodel import ProjectModel
from ._removecommand import RemoveCommand
from ._role import Role
from ._setcommand import SetCommand




def initialize():
    """
    Detailed description.
    """
    global speller
    speller = hunspell.HunSpell(
        os.path.join(settings.HUNSPELL_ROOT,settings.DICTIONARY+".dic")
        ,os.path.join(settings.HUNSPELL_ROOT,settings.DICTIONARY+".aff")
    )


#
# Detailed description.
#
blockFactory = BlockFactory()


#
# Detailed description.
#
speller = None


#
# Detailed description.
#
parser = ParserModel()
