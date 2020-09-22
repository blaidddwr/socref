"""
Contains the Templatee class.
"""
from socref import edit
from ._base import Base








class Templatee(Base):
    """
    This is the templatee class. It provides methods for handling template block
    children. This is meant to act as a base class for any block that can have
    templates.
    """


    def __init__(
        self
        ):
        """
        Initializes a new templatee block.
        """
        Base.__init__(self)


    def hasTemplates(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : bool
               True if this block has any templates or false otherwise.
        """
        for child in self:
            if child._TYPE_ == "Template":
                return True
        return False


    def templateDeclaration(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               Template declaration code of this block based off its child
               template blocks or an empty string if it has no templates. The
               returned declaration includes any initial value of template
               children.
        """
        ret = self.__declaration_(True,True)
        return "template"+ret if ret else ""


    def templateDefinition(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               Template definition code of this block based off its child
               template blocks or an empty string if it has no templates. The
               returned declaration does not include any initial value of
               template children.
        """
        ret = self.__declaration_(True,False)
        return "template"+ret if ret else ""


    def templateScope(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               Template scope code of this block based off its child template
               blocks or an empty string if it has no templates.
        """
        return self.__declaration_(False,False)


    def _templatesName_(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               A decoration for this block's display name providing information
               about its templates. If this block has no templates then an empty
               string is returned.
        """
        return " <>" if self.hasTemplates() else ""


    def _templatesView_(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : rich text
               Providing detailed information about this block's templates. If
               this block has no templates then an empty string is returned.
        """
        return edit.richText(
            2
            ,"Templates"
            ,"".join((child.argumentView() for child in self if child._TYPE_ == "Template"))
        )


    def __declaration_(
        self
        ,withType
        ,withInit
        ):
        """
        Getter method.

        Parameters
        ----------
        withType : bool
                   True to include the template type in the returned template
                   code string or false otherwise.
        withInit : bool
                   True to include any initial value of a template argument or
                   false otherwise.

        Returns
        -------
        ret0 : string
               Template code that is the list of this block's children
               templates, optionally including the template type and/or any
               initial value. If this block has no templates then an empty
               string is returned. The returned code does not include the
               initial "template" keyword for declarations.
        """
        args = []
        for child in self:
            if child._TYPE_ == "Template":
                line = ""
                if withType:
                    line = child._p_type.replace("@",child._p_name)
                else:
                    line = child._p_name
                if withInit and child._p_assignment:
                    line += "="+child._p_assignment
                args.append(line)
        if args:
            return "<%s>" % ",".join(args)
        else:
            return ""
