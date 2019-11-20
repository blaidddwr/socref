"""
Detailed description.
"""
import os
import re
from PySide2 import QtCore as qtc
from PySide2 import QtGui as qtg
import hunspell








class Spell_Highlighter(qtg.QSyntaxHighlighter):
    """
    Detailed description.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, dictionary, parent):
        """
        Detailed description.

        dictionary : Detailed description.

        parent : Detailed description.
        """
        qtg.QSyntaxHighlighter.__init__(self,parent)
        self.__hunspell = hunspell.HunSpell(os.path.join(self.__HUN_ROOT,dictionary + ".dic")
                                            ,os.path.join(self.__HUN_ROOT,dictionary + ".aff"))
        format_ = self.__format = qtg.QTextCharFormat()
        format_.setFontUnderline(True)
        format_.setUnderlineColor(qtc.Qt.red)
        format_.setUnderlineStyle(qtg.QTextCharFormat.WaveUnderline)


    ####################
    # PUBLIC - Methods #
    ####################


    def highlightBlock(self, text):
        """
        Detailed description.

        text : Detailed description.
        """
        start = 0
        pattern = re.compile('\w+')
        while True :
            match = pattern.search(text,start)
            if not match : break
            if not self.__hunspell.spell(match.group(0)) :
                self.setFormat(match.start(),match.end() - match.start(),self.__format)
            start = match.end()


    ############################
    # PRIVATE - Static Objects #
    ############################


    #
    # Detailed description.
    #
    __HUN_ROOT = "/usr/share/hunspell"
