"""
Contains all utility functions and classes that provide basic utilities to the rest of the core
application and language implementations.
"""




def wrap_blocks(text, begin="", separator="", columns=80):
    """
    Wraps the given text into separate blocks that are in turn wrapped into multiple lines with the
    given begin string appended to each one and limited by the given number of columns per line. The
    delimiter for each given block is double new lines. The output of each block is separated by a
    line with the given separator string.

    text : A string of text that is wrapped into multiple lines and separated into blocks.

    begin : A string that is appended to every line of wrapped text generated.

    separator : A string of text that is used to separate each block of text.

    columns : The maximum column length for each line of wrapped text.

    return : Wrapped lines of text separated into multiple blocks of text using a given separator
             string.
    """
    return "\n".join((wrap_text(block,begin=begin,columns=columns) for block in text.split("\n\n") if block))




def wrap_text(text, begin="", after="", columns=80):
    """
    Wraps given text into multiple lines that are limited by the given number of columns per line.
    Every line is appended with an optional begin string and every line after the first is also
    appended with an optional after line.

    text : A string of text that is wrapped into multiple lines.

    begin : A string that is appended to every line of wrapped text generated.

    after : A string of text that is appended to every wrapped line of text after the begin string.

    columns : The maximum column length for each line of wrapped text.

    return : New line separated string of wrapped text generated from the given input text.
    """
    ret = ""
    words = text.split()
    first = True
    while words :
        if first :
            line = begin + words.pop(0)
            first = False
        else: line = begin + after + words.pop(0)
        while words and (len(line) + len(words[0]) + 1) <= columns :
            line += " " + words.pop(0)
        ret += line + "\n"
    return ret




def line_edit(label, key):
    """
    Returns an edit definition dictionary for a line edit. This is used with the abstract block's
    edit definitions interface.

    label : The label string for the edit definition.

    key : The key string for the edit definition.

    return : Edit definition dictionary for a line edit.
    """
    return __edit("line",label,key)




def text_edit(label, key, speller=False):
    """
    Returns an edit definition dictionary for a text edit. This is used with the abstract block's
    edit definitions interface.

    label : The label string for the edit definition.

    key : The key string for the edit definition.

    speller : Optional speller Boolean for the edit definition.

    return : Edit definition dictionary for a text edit.
    """
    ret = __edit("text",label,key)
    ret["speller"] = speller
    return ret




def checkbox_edit(label, key):
    """
    Returns an edit definition dictionary for a checkbox edit. This is used with the abstract
    block's edit definitions interface.

    label : The label string for the edit definition.

    key : The key string for the edit definition.

    return : Edit definition dictionary for a checkbox edit.
    """
    return __edit("checkbox",label,key)




def combobox_edit(label, key):
    """
    Returns an edit definition dictionary for a combo box edit with no selection values. This is
    used with the abstract block's edit definitions interface.

    label : The label string for the edit definition.

    key : The key string for the edit definition.

    return : Edit definition dictionary for a combo box edit.
    """
    ret = __edit("combobox",label,key)
    ret["selections"] = []
    return ret




def add_combo_select(combo_edit, text, icon=None):
    """
    Adds a selection value to the given combo box edit definition with the given text and optional
    icon.

    combo_edit : A combo box edit definition dictionary that has a new selection value added to it.

    text : The text of the new selection value.

    icon : The optional qt icon of the new selection value.
    """
    edit = {"text": text}
    if icon is not None : edit["icon"] = icon
    combo_edit["selections"].append(edit)




def hidden_edit(key, value):
    """
    Returns an edit definition dictionary for a hidden edit. This is used with the abstract block's
    edit definitions interface.

    key : The key string for the edit definition.

    value : The value string for the edit definition.

    return : Edit definition dictionary for a hidden edit.
    """
    return {"type": "hidden"
            ,"key": key
            ,"value": value}








class Singleton():
    """
    This is the descriptor class that turns a class used by it to a singleton. It provides a class
    method that returns the class object of the singleton instance.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, class_):
        """
        Initializes a new singleton descriptor with the given class.

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








def __edit(type_, label, key):
    """
    Returns a generic edit definition dictionary with the given type, label, and key.

    type_ : The type of the generic edit definition.

    label : The label of the generic edit definition.

    key : The key of the generic edit definition.

    return : Generic edit definition dictionary.
    """
    return {"type": type_
            ,"label": label
            ,"key": key}
