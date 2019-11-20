"""
Contains all utility functions and classes that provide basic utilities to the rest of the program.
"""




def wrap_text(text, begin="", after="", columns=80):
    """
    Detailed description.

    text : Detailed description.

    begin : Detailed description.

    after : Detailed description.

    columns : Detailed description.

    return : Yes
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
    Detailed description.

    label : Detailed description.

    key : Detailed description.

    return : Yes
    """
    return __edit("line",label,key)




def text_edit(label, key, speller=False):
    """
    Detailed description.

    label : Detailed description.

    key : Detailed description.

    speller : Detailed description.

    return : Yes
    """
    ret = __edit("text",label,key)
    ret["speller"] = speller
    return ret




def checkbox_edit(label, key):
    """
    Detailed description.

    label : Detailed description.

    key : Detailed description.

    return : Yes
    """
    return __edit("checkbox",label,key)




def combobox_edit(label, key):
    """
    Detailed description.

    label : Detailed description.

    key : Detailed description.

    return : Yes
    """
    ret = __edit("combobox",label,key)
    ret["selections"] = []
    return ret




def add_combo_select(combo_edit, text, icon=None):
    """
    Detailed description.

    combo_edit : Detailed description.

    text : Detailed description.

    icon : Detailed description.
    """
    edit = {"text": text}
    if icon is not None : edit["icon"] = icon
    combo_edit["selections"].append(edit)




def hidden_edit(key, value):
    """
    Detailed description.

    key : Detailed description.

    value : Detailed description.

    return : Yes
    """
    return {"type": "hidden"
            ,"key": key
            ,"value": value}








class Singleton():
    """
    This is a descriptor class that will turn a class used by it to a singleton. The singleton is
    called by calling its class object which is replaced by this and the same instance is called
    every time.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


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
        Returns the singleton instance that this singleton instance manages.

        return : The singleton instance of the class that this singleton manages.
        """
        return self.__instance


    ####################
    # PUBLIC - Methods #
    ####################


    def class_(self):
        """
        Returns the class object that this singleton instance manages.

        return : The class object that this singleton instance manages.
        """
        return self.__class








def __edit(type_, label, key):
    """
    Detailed description.

    type_ : Detailed description.

    label : Detailed description.

    key : Detailed description.

    return : Yes
    """
    return {"type": type_
            ,"label": label
            ,"key": key}
