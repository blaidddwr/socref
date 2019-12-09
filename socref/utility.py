"""
Contains all utility functions and classes that provide basic utilities to the rest of the core
application and language implementations.
"""
import html




def addComboSelect(comboEdit, text, icon=None):
    """
    Adds a selection to the given combo box edit definition with the given text and optional icon.

    comboEdit : A combo box edit definition dictionary that has a new selection value added to it.

    text : The text of the new selection value.

    icon : The optional qt icon of the new selection value.
    """
    edit = {"text": text}
    if icon is not None:
        edit["icon"] = icon
    comboEdit["selections"].append(edit)




def checkboxEdit(label, key):
    """
    Build method.

    label : The label string for the edit definition.

    key : The key string for the edit definition.

    return : An abstract block's edit definition for a checkbox edit.
    """
    return edit("checkbox",label,key)




def comboEdit(label, key):
    """
    Build method.

    label : The label string for the edit definition.

    key : The key string for the edit definition.

    return : An abstract block's edit definition for a combo box edit with no selections.
    """
    ret = edit("combobox",label,key)
    ret["selections"] = []
    return ret




def edit(type_, label, key):
    """
    Builder method.

    type_ : The type of the generic edit definition.

    label : The label of the generic edit definition.

    key : The key of the generic edit definition.

    return : A generic abstract block's edit definition.
    """
    return {"type": type_
            ,"label": label
            ,"key": key}




def hiddenEdit(key, value):
    """
    Build method.

    key : The key string for the edit definition.

    value : The value string for the edit definition.

    return : An abstract block's edit definition for a hidden edit.
    """
    return {"type": "hidden"
            ,"key": key
            ,"value": value}




def lineEdit(label, key):
    """
    Build method.

    label : The label string for the edit definition.

    key : The key string for the edit definition.

    return : An abstract block's edit definition for a line edit.
    """
    return edit("line",label,key)




def richText(size, title, body):
    """
    Builder function.

    size : The size of the header, if any, following the HTML standard.

    title : The title used for the HTML header.

    body : The body text that is wrapped in the returned rich text if it is not empty.

    return : A rich text string generated from the given body, header, and size. The body is wrapped
             in an HTML paragraph and an HTML header added before it. If the given body is empty
             then an empty string is returned.
    """
    ret = ""
    if body:
        header = "h" + str(size) + ">"
        ret = "<" + header + title + "</" + header + body
    return ret




def richTextBlocks(size, title, body):
    """
    Builder function.

    size : See rich text function.

    title : See rich text function.

    body : Same as rich text function with the added step of splitting it into paragraphs.

    return : The rich text function's return but with the body being divided into paragraphs using
             two new lines as the delimiter.
    """
    return richText(
        size
        ,title
        ,"".join(("<p>%s</p>" % block for block in body.split("\n\n") if block))
    )




def richTextList(size, title, list_):
    """
    Builder function.

    size : See rich text function.

    title : See rich text function.

    list_ : A list of strings that are combined as an unordered HTML list.

    return : The rich text function's return but with the body being replaced by the list that is
             formatted as an unordered HTML list. The list being empty is the same of the body being
             empty.
    """
    htmlList = "</li><li>".join(list_)
    if htmlList:
        htmlList = "<ul><li>%s</li></ul>" % htmlList
    return richText(size,title,htmlList)




def textEdit(label, key, speller=False):
    """
    Build method.

    label : The label string for the edit definition.

    key : The key string for the edit definition.

    speller : True to enable spelling or false to disable it.

    return : An abstract block's edit definition for a text edit.
    """
    ret = edit("text",label,key)
    ret["speller"] = speller
    return ret




def wrapBlocks(text, begin="", separator="", columns=80):
    """
    Builder function.

    text : Same as wrap text function.

    begin : Same as wrap text function.

    separator : A string of text that is used to separate each paragraph of text.

    columns : Same as wrap text function.

    return : The wrap text function's returned but with paragraphs being separated by the given
             string.
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

    text : A string of text that is wrapped into multiple lines.

    begin : A string that is appended to every line of wrapped text generated.

    after : A string of text that is appended to every wrapped line of text after the begin string.

    columns : The maximum column length for each line of wrapped text.

    return : Wrapped text generated from the given text, optional begin and after strings, and
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

        class_ : Class object that is made into a singleton class.
        """
        self.__class = class_
        self.__instance = class_()


    ######################
    # PUBLIC - Operators #
    ######################


    def __call__(self):
        """
        Implements the call operator.

        return : The singleton instance of this descriptor's class.
        """
        return self.__instance


    ####################
    # PUBLIC - Methods #
    ####################


    def class_(self):
        """
        Getter method.

        return : The class object of this descriptor.
        """
        return self.__class
