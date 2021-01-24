"""
Contains the SpellHighlighter class.
"""
from ...Model import speller
from PySide2.QtCore import Qt
from PySide2.QtGui import QSyntaxHighlighter
from PySide2.QtGui import QTextCharFormat
from re import reCompile




class SpellHighlighter(QSyntaxHighlighter):
    """
    This is the spell highlighter class. It implements the qt syntax highlighter
    class. It provides highlighting of misspelled words in the document the
    highlighter is given on initialization. It uses the Hunspell library for all
    speller functions.
    """


    def __init__(
        self
        ,dictionary
        ,parent
    ):
        """
        Initializes a new spell highlighter with the given dictionary and
        parent.

        Parameters
        ----------
        dictionary : string
                     The Hunspell dictionary used for spell checking. This does
                     not include the directory or file extensions, for example
                     "en_US".
        parent : PySide2.QtGui.QTextDocument
                 The parent document that takes ownership of this highlighter
                 and has its misspelled words highlighted.
        """
        super().__init__(parent)
        format_ = self.__format = QTextCharFormat()
        format_.setFontUnderline(True)
        format_.setUnderlineColor(Qt.red)
        format_.setUnderlineStyle(QTextCharFormat.WaveUnderline)


    def highlightBlock(
        self
        ,text
    ):
        start = 0
        pattern = reCompile('\w+')
        while True:
            match = pattern.search(text,start)
            if not match:
                break
            if not speller.spell(match.group(0)):
                self.setFormat(match.start(),match.end() - match.start(),self.__format)
            start = match.end()
