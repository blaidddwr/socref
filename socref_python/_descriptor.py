"""
Contains the Descriptor class.
"""
import html
from socref import edit
from ._package import Package








class Descriptor(Package):
    """
    This is the descriptor class. It partially implements the Socrates' Reference abstract block
    class. This provides a descriptors property along with utility methods for it. This is meant to
    act as a base class for any block that has descriptors.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(
        self
        ):
        """
        Initializes a new descriptor block.
        """
        Package.__init__(self)
        self._p_descriptors = ""


    ####################
    # PUBLIC - Methods #
    ####################


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Package.clearProperties(self)
        self._p_descriptors = ""


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Package.setDefaultProperties(self)
        self._p_descriptors = ""


    #######################
    # PROTECTED - Methods #
    #######################


    def _buildDescriptors_(
        self
        ,begin
        ):
        """
        Getter method.

        Parameters
        ----------
        begin : string
                The indent that is added to the beginning of every line of returned code.

        Returns
        -------
        ret0 : string
               The source code for this block's descriptors. If this block has no descriptors an
               empty string is returned.
        """
        return [begin + "@" + line for line in self._p_descriptors.split("\n") if line]


    def _descriptorsEditDefinition_(
        self
        ):
        """
        Getter Method.

        Returns
        -------
        ret0 : dictionary
               The edit definition for this block's descriptors property.
        """
        return edit.textEdit("Descriptors:","_p_descriptors")


    def _descriptorsName_(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               A decoration for a block's display name providing information about its descriptors.
               If this block has no descriptors then an empty string is returned.
        """
        if self._p_descriptors:
            return " @"
        else:
            return ""


    def _descriptorsView_(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : rich text
               Detailed information about this block's descriptors. If this block has no descriptors
               then an empty string is returned.
        """
        return edit.richText(
            2
            ,"Descriptors"
            ,"".join(
                ("<p>@%s</p>" % html.escape(line)
                 for line in self._p_descriptors.split("\n") if line)
             )
        )
