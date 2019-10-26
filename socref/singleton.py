





def singleton(class_):
    """
    todo
    """
    instances = {}
    def wrapper():
        if class_ not in instances.keys():
            instances[class_] = class_();
        return instances[class_]
    return wrapper
