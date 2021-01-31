"""
Detailed description.
"""
from ... import HUNSPELL_ROOT
from ... import DICTIONARY
from hunspell import HunSpell
from os.path import join as pathJoin


speller = HunSpell(
    pathJoin(HUNSPELL_ROOT,DICTIONARY+".dic")
    ,pathJoin(HUNSPELL_ROOT,DICTIONARY+".aff")
)
