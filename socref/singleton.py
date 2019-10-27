"""
todo
"""






def singleton(class_):
    instances = {}
    def wrapper():
        if class_ not in instances.keys():
            instances[class_] = class_();
        return instances[class_]
    return wrapper
