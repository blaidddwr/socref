"""
todo
"""






class Singleton():


    def __init__(self, class_):
        #
        self.__class = class_
        #
        self.__instance = class_()
        #:


    def __call__(self):
        return self.__instance


    def class_(self):
        return self.__class
