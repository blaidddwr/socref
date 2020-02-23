"""
Contains all utility functions and classes that provide basic utilities to the rest of the core
application and language implementations.
"""
import html




def addComboSelect(comboEdit, text, icon=None):
    """
    Adds a selection to the given combo box edit definition with the given text and optional icon.

    Parameters
    ----------
    comboEdit : dictionary
                A combo box edit definition dictionary that has a new selection value added to it.
    text : string
           The text of the new selection value.
    icon : PySide2.QtGui.QIcon
           The optional qt icon of the new selection value.
    """
    edit = {"text": text}
    if icon is not None:
        edit["icon"] = icon
    comboEdit["selections"].append(edit)




def checkboxEdit(label, key):
    """
    Builder function.

    Parameters
    ----------
    label : string
            The label for the edit definition.
    key : string
          The key for the edit definition.

    Returns
    -------
    ret0 : dictionary
           An abstract block's edit definition for a checkbox edit.
    """
    return edit("checkbox",label,key)




def comboEdit(label, key):
    """
    Builder function.

    Parameters
    ----------
    label : string
            The label for the edit definition.
    key : string
          The key for the edit definition.

    Returns
    -------
    ret0 : dictionary
           An abstract block's edit definition for a combo box edit with no selections.
    """
    ret = edit("combobox",label,key)
    ret["selections"] = []
    return ret




def edit(type_, label, key):
    """
    Builder function.

    Parameters
    ----------
    type_ : string
            The type of the generic edit definition.
    label : string
            The label of the generic edit definition.
    key : string
          The key of the generic edit definition.

    Returns
    -------
    ret0 : dictionary
           A generic abstract block's edit definition.
    """
    return {"type": type_
            ,"label": label
            ,"key": key}




def hiddenEdit(key, value):
    """
    Builder function.

    Parameters
    ----------
    key : string
          The key for the edit definition.
    value : string
            The value for the edit definition.

    Returns
    -------
    ret0 : dictionary
           An abstract block's edit definition for a hidden edit.
    """
    return {"type": "hidden"
            ,"key": key
            ,"value": value}




def lineEdit(label, key):
    """
    Builder function.

    Parameters
    ----------
    label : string
            The label for the edit definition.
    key : string
          The key for the edit definition.

    Returns
    -------
    ret0 : dictionary
           An abstract block's edit definition for a line edit.
    """
    return edit("line",label,key)




def richText(size, title, body):
    """
    Builder function.

    Parameters
    ----------
    size : int
           The size of the header, if any, following the HTML standard.
    title : string
            The title used for the HTML header.
    body : string
           The body that is wrapped in the returned rich text if it is not empty.

    Returns
    -------
    ret0 : rich text
           Generated from the given body, header, and size. The body is wrapped in an HTML paragraph
           and an HTML header added before it. If the given body is empty then an empty string is
           returned.
    """
    ret = ""
    if body:
        header = "h" + str(size) + ">"
        ret = "<" + header + title + "</" + header + body
    return ret




def richTextBlocks(size, title, body):
    """
    Builder function.

    Parameters
    ----------
    size : int
           See rich text function.
    title : string
            See rich text function.
    body : string
           Same as rich text function with the added step of splitting it into paragraphs.

    Returns
    -------
    ret0 : rich text
           The richText function's return but with the body being divided into paragraphs using two
           new lines as the delimiter.
    """
    return richText(
        size
        ,title
        ,"".join(("<p>%s</p>" % block for block in body.split("\n\n") if block))
    )




def richTextList(size, title, list_):
    """
    Builder function.

    Parameters
    ----------
    size : int
           See rich text function.
    title : string
            See rich text function.
    list_ : list
            Strings that are combined as an unordered HTML list.

    Returns
    -------
    ret0 : rich text
           The richText function's return but with the body being replaced by the list that is
           formatted as an unordered HTML list. The list being empty is the same of the body being
           empty.
    """
    htmlList = "</li><li>".join(list_)
    if htmlList:
        htmlList = "<ul><li>%s</li></ul>" % htmlList
    return richText(size,title,htmlList)




def textEdit(label, key, speller=False):
    """
    Builder function.

    Parameters
    ----------
    label : string
            The label for the edit definition.
    key : string
          The key for the edit definition.
    speller : bool
              True to enable spelling or false to disable it.

    Returns
    -------
    ret0 : dictionary
           An abstract block's edit definition for a text edit.
    """
    ret = edit("text",label,key)
    ret["speller"] = speller
    return ret




def uniqueInsert(dict_, key, item):
    """
    Inserts the given item into the given dictionary with the given key. If the given key already
    exists in the dictionary a number is appended to the key string until a key is found that does
    not exist in the dictionary.

    Parameters
    ----------
    dict_ : dictionary
            Has an item inserted into it.
    key : string
          The key used to insert the given item into the given dictionary. If the key already exists
          this is modified.
    item : object
           The item that is inserted into the given dictionary.
    """
    if key in dict_:
        count = 0
        key = key + str(count)
        while key in dict_:
            count += 1
            key = key[:-1] + str(count)
    dict_[key] = item




def wrapBlocks(text, begin="", separator="", columns=80):
    """
    Builder function.

    Parameters
    ----------
    text : string
           Same as wrap text function.
    begin : string
            Same as wrap text function.
    separator : string
                Used to separate each paragraph of text.
    columns : int
              Same as wrap text function.

    Returns
    -------
    ret0 : list
           The wrapText function's returned but with paragraphs being separated by the given line.
    """
    ret = []
    first = True
    for block in text.split("\n\n"):
        if block:
            if not first:
                ret += [""]
            else:
                first = False
            ret += wrapText(block,begin,columns=columns)
    return ret




def wrapText(text, begin="", after="", columns=80):
    """
    Builder function.

    Parameters
    ----------
    text : string
           Text that is wrapped into multiple lines.
    begin : string
            Added to the beginning of every line of wrapped text generated.
    after : string
            Added to the beginning of every wrapped line of text, excluding the first line, after
            the begin string.
    columns : int
              The maximum column length for each line of wrapped text.

    Returns
    -------
    ret0 : list
           Wrapped string lines generated from the given text, optional begin and after strings, and
           maximum line length.
    """
    ret = []
    words = text.split()
    first = True
    while words:
        if first:
            line = begin + words.pop(0)
            first = False
        else:
            line = begin + after + words.pop(0)
        while words and (len(line) + len(words[0]) + 1) <= columns:
            line += " " + words.pop(0)
        ret.append(line)
    return ret








class Singleton():
    """
    This is the descriptor singleton class. It turns its descriptor class into a singleton. It
    provides a class method that returns the class object of the singleton instance.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, class_):
        """
        Initializes a new singleton with the given class object.

        Parameters
        ----------
        class_ : class
                 Class object that is made into a singleton class.
        """
        self.__class = class_
        self.__instance = class_()


    ######################
    # PUBLIC - Operators #
    ######################


    def __call__(self):
        """
        Implements the call operator.

        Returns
        -------
        ret0 : instance
               The singleton instance of this descriptor's class.
        """
        return self.__instance


    ####################
    # PUBLIC - Methods #
    ####################


    def class_(self):
        """
        Getter method.

        Returns
        -------
        ret0 : class
               The class object of this descriptor.
        """
        return self.__class
