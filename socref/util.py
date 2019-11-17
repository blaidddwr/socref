"""
todo
"""




def wrap_text(text,begin="",after="",columns=80):
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
    return __edit("line",label,key)




def text_edit(label, key):
    return __edit("text",label,key)




def checkbox_edit(label, key):
    return __edit("checkbox",label,key)




def combobox_edit(label, key):
    ret = __edit("combobox",label,key)
    ret["selections"] = []
    return ret




def add_combo_select(combo_edit, text, icon=None):
    edit = {"text": text}
    if icon is not None : edit["icon"] = icon
    combo_edit["selections"].append(edit)




def hidden_edit(key, value):
    return {"type": "hidden"
            ,"key": key
            ,"value": value}




def __edit(type_, label, key):
    return {"type": type_
            ,"label": label
            ,"key": key}
