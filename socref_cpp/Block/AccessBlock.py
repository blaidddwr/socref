"""
Contains the AccessBlock class.
"""
from PySide6.QtGui import QIcon
from socref import block
from socref.Base.BlockBase import BlockBase
from socref.Edit.ComboEdit import ComboEdit
from socref.Output.RichText import RichText




@block("Access")
class AccessBlock(BlockBase):
    """
    This is the access block class. It implements the Socrates' Reference
    abstract block class. It represents a C++ class access segment.
    """
    TYPES = (
        ("Public",QIcon(":/socref_cpp/public.svg"))
        ("Protected",QIcon(":/socref_cpp/protected.svg"))
        ("Private",QIcon(":/socref_cpp/private.svg"))
    )
    TYPES_LOOKUP = dict(TYPES)


    def __init__(
        self
    ):
        super().__init__()
        self._p_type = "Public"


    def buildList(
        self
    ):
        #return ("Enumeration","Function","Class","Union")
        return ("Enumeration","Function","Class")


    def clearProperties(
        self
    ):
        self._p_type = "Public"


    def displayName(
        self
    ):
        return self._p_type + " (%i)"%len(self)


    def displayView(
        self
    ):
        ret = RichText()
        ret.addHeader(self._p_type,1)
        return ret


    def editDefinitions(
        self
    ):
        return [ComboEdit("Name:","_p_name",self.TYPES)]


    def hasAbstract(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this access block contains any abstract methods or
                 false otherwise.
        """
        for child in self:
            if child._TYPE_ == "Function":
                if child.isAbstract():
                    return True
        return False


    def hasVirtual(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this access block contains any virtual methods or false
                 otherwise.
        """
        for child in self:
            if child._TYPE_ == "Function":
                if child.isVirtual():
                    return True
        return False


    def icon(
        self
    ):
        return self.TYPES_LOOKUP[self._p_type]


    def isVolatileAbove(
        self
    ):
        return True


    def setDefaultProperties(
        self
    ):
        self._p_type = "Public"


    def lower(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : string
                 The all lowercase type of this access block. Possible types are
                 public, protected, and private.
        """
        return self._p_type.lower()
