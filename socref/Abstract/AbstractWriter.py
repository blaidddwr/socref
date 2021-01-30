"""
Contains the AbstractWriter class.
"""
from . import AbstractParser
from abc import ABC
from abc import abstractmethod




class AbstractWriter(ABC):
    """
    Detailed description.
    """


    def __init__(
        self
        ,parent
    ):
        """
        asdf

        Parameters
        ----------
        parent : object
                 Detailed description.
        """
        if isinstance(parent,AbstractWriter):
            self.__parser = parent.__parser
            self.__parent.__children.append(self)
        elif isinstance(parent,AbstractParser.AbstractParser):
            self.__parser = parent
        else:
            raise ScanError("Given parent is not an abstract parser or writer.")
        self.__children = []
        self.__base = []


    def __call__(
        self
    ):
        """
        Detailed description.
        """
        ret = self._header_()
        for child in self.__children:
            ret += child()
        ret = self._footer_()
        return ret


    def lookup(
        self
        ,*keys
    ):
        """
        Detailed description.

        Parameters
        ----------
        *keys : object
                Detailed description.
        """
        return self.__parser.lookup(*(self.__base+keys))


    def setLookupBase(
        self
        ,*keys
    ):
        """
        Detailed description.

        Parameters
        ----------
        *keys : object
                Detailed description.
        """
        self.__base = keys


    @abstractmethod
    def _footer_(
        self
    ):
        """
        Detailed description.
        """
        pass


    @abstractmethod
    def _header_(
        self
    ):
        """
        Detailed description.
        """
        pass
