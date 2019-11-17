"""
Holds the singleton class, this will change.
"""








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
        #
        # Initialize the given class and then the singleton instance of it.
        #
        self.__class = class_
        self.__instance = class_()


    ######################
    # PUBLIC - Operators #
    ######################


    def __call__(self):
        """
        Returns the singleton instance that this singleton instance manages.

        The singleton instance of the class that this singleton manages.
        """
        return self.__instance


    ###########################
    # PUBLIC - Public Methods #
    ###########################


    def class_(self):
        """
        Returns the class object that this singleton instance manages.

        The class object that this singleton instance manages.
        """
        return self.__class
