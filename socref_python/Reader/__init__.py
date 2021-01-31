"""
Detailed description.
"""


def skipDocString(
    reader
):
    """
    Detailed description.

    Parameters
    ----------
    reader : object
             Detailed description.
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
