"""
Contains the Role class.
"""
import enum
from PySide2 import QtCore as qtc








class Role(enum.Enum):
    """
    This is the role enumeration class. It defines all custom data roles the project model
    implements.
    """


    #########################
    # PUBLIC - Enumerations #
    #########################


    #
    # Defines the block type role which represents a block's type name.
    #
    BLOCK_TYPE = qtc.Qt.UserRole + 0


    #
    # Defines the view role which represents a block's detailed rich text view.
    #
    VIEW = qtc.Qt.UserRole + 1


    #
    # Defines the build list role which represents a block's build list.
    #
    BUILD_LIST = qtc.Qt.UserRole + 2


    #
    # Defines the properties role which represents a block's properties.
    #
    PROPERTIES = qtc.Qt.UserRole + 3


    #
    # Defines the edit definitions role which represents a block's edit definitions.
    #
    EDIT_DEFS = qtc.Qt.UserRole + 4
