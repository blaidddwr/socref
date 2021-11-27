"""
Contains the PythonReaderBase class.
"""
from socref.Base.ReaderBase import ReaderBase




class PythonReaderBase(ReaderBase):
    """
    This is the python reader base class. It partially implements the Socrates'
    Reference abstract reader class. It provides common methods used by multiple
    other python reader classes.
    """


    def _skipDocString_(
        self
    ):
        """
        Skips a document string block of lines if one is encountered. If a line
        of code that is not a document string is encountered then the reader's
        position is at that line, else its position is the first line after the
        encountered document string.
        """
        count = 0
        while count < 2:
            self.save()
            (i,line) = self.read()
            if line is None:
                return
            if line == '"""':
                count += 1
            elif line and count == 0:
                self.restore()
                return
            self.discard()
