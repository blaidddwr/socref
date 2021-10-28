"""
Contains all implemented Socrates' Reference reader classes.
"""


def skipDocString(
    reader
):
    """
    Skips a document string block of lines in the given reader if one is
    encountered. If a line of code that is not a document string is encountered
    then the reader's position is at that line, else its position is the first
    line after the encountered document string.

    Parameters
    ----------
    reader : AbstractReader
             The reader.
    """
    count = 0
    while count < 2:
        reader.save()
        (i,line) = reader.read()
        if line is None:
            return
        if line == '"""':
            count += 1
        elif line and count == 0:
            reader.restore()
            return
        reader.discard()
