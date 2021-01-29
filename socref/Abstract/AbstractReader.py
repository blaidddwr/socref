"""
Contains the AbstractReader class.
"""
from . import AbstractParser
from ..Error.ScanError import *
from abc import ABC
from abc import abstractmethod




class AbstractReader(ABC):
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
        self.__parent = parent
        if isinstance(parent,AbstractReader):
            self.__parser = parent.__parser
        elif isinstance(parent,AbstractParser.AbstractParser):
            self.__parser = parent
        else:
            raise ScanError("Given parent is not an abstract parser or reader.")
        self.__key = None
        self.__lookups = {}


    def __call__(
        self
    ):
        """
        Detailed description.
        """
        self._scan_()
        if isinstance(self.__parent,AbstractReader):
            if self.__key is None:
                raise ScanError("An abstract reader failed to set its key after scanning.")
            if self.__key in self.__parent.__lookups:
                raise ScanError("An abstract reader set a duplicate key.")
            self.__parent.__lookups[self.__key] = self


    def __getitem__(
        self
        ,key
    ):
        return self.__lookups.get(key,None)


    def __iter__(
        self
    ):
        """
        Detailed description.
        """
        return self.__lookups.__iter__()


    def peak(
        self
    ):
        """
        Detailed description.
        """
        return self.__parser.peak()


    def read(
        self
    ):
        """
        Detailed description.
        """
        return self.__parser.read()


    def restore(
        self
    ):
        """
        Detailed description.
        """
        self.__parser.restore()


    def save(
        self
    ):
        """
        Detailed description.
        """
        self.__parser.save()


    @abstractmethod
    def unknown(
        self
    ):
        """
        Detailed description.
        """
        pass


    @abstractmethod
    def _scan_(
        self
    ):
        """
        Detailed description.
        """
        pass


    def _setKey_(
        self
        ,key
    ):
        """
        Detailed description.

        Parameters
        ----------
        key : object
              Detailed description.
        """
        if self.__key is not None:
            raise ScanError("An abstract reader attempted to overwrite its key.")
        self.__key = key
