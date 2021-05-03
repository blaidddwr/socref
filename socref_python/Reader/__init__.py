"""
Contains all implemented Socrates' Reference reader classes.
"""


def skipDocString(
    reader
):
    """
    Skips a doc string block of lines from the given reader if one is
    encountered. If a line of code that is not a doc string is encountered then
    the reader's position is at that line, else its position is the first line
    after the encountered doc string.

    Parameters
    ----------
    reader : socref.Abstract.AbstractReader
             The reader whose line position is moved to after an encountered doc
             string or the first line of actual code.
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
