"""
Contains the SpellHighlighter class.
"""
import re
from PySide2 import QtCore as qtc
from PySide2 import QtGui as qtg
from . import core








class SpellHighlighter(qtg.QSyntaxHighlighter):
    """
    This is the spell highlighter class. It implements the qt syntax highlighter class. It provides
    highlighting of misspelled words in the document the highlighter is given on initialization. It
    uses the Hunspell library for all speller functions.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, dictionary, parent):
        """
        Initializes a new spell highlighter with the given dictionary and parent.

        Parameters
        ----------
        dictionary : string
                     The Hunspell dictionary used for spell checking. This does not include the
                     directory or file extensions, for example "en_US".
        parent : PySide2.QtGui.QTextDocument
                 The parent document that takes ownership of this highlighter and has its misspelled
                 words highlighted.
        """
        qtg.QSyntaxHighlighter.__init__(self,parent)
        format_ = self.__format = qtg.QTextCharFormat()
        format_.setFontUnderline(True)
        format_.setUnderlineColor(qtc.Qt.red)
        format_.setUnderlineStyle(qtg.QTextCharFormat.WaveUnderline)


    ####################
    # PUBLIC - Methods #
    ####################


    def highlightBlock(self, text):
        """
        Implements the qtg.QSyntaxHighlighter interface.

        Parameters
        ----------
        text : object
               See qt docs.
        """
        start = 0
        pattern = re.compile('\w+')
        while True:
            match = pattern.search(text,start)
            if not match:
                break
            if not core.speller.spell(match.group(0)):
                self.setFormat(match.start(),match.end() - match.start(),self.__format)
            start = match.end()
