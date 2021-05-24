"""
Contains the FunctionBlock class.
"""
from .ProgramBlock import *
from PySide6.QtGui import QIcon
from socref import block
from socref.Edit.LineEdit import *
from socref.Edit.TextEdit import *




@block("Function")
class FunctionBlock(ProgramBlock):
    """
    This is the function block class. It implements the Socrates' Reference
    abstract block class. It represents a GLSL function.
    """


    def __init__(
        self
    ):
        super().__init__()
        self._p_returnType = ""
        self._p_returnDescription = ""


    def arguments(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : list
                 Tuples of argument values for this function. Each tuple
                 contains the name, type, and description in that order.
        """
        ret = []
        for variable in self:
            ret.append((variable._p_name,variable._p_type.replace("@",""),variable._p_description))
        return ret


    def buildList(
        self
    ):
        return ("Variable",)


    def clearProperties(
        self
    ):
        super().clearProperties()
        self._p_returnType = ""
        self._p_returnDescription = ""


    def displayName(
        self
    ):
        return self._p_name+"("+str(len(self))+")"


    def displayView(
        self
    ):
        ret = super().displayView()
        args = self.arguments()
        if args:
            ret.addHeader("Arguments",1)
            for (name,type_,description) in args:
                ret.addBox(name+" : "+type_,description)
        if self._p_returnDescription:
            ret.addHeader("Return",1)
            ret.addBox(self._p_returnType,self._p_returnDescription)
        return ret


    def editDefinitions(
        self
    ):
        ret = super().editDefinitions()
        ret.append(LineEdit("Return Type:","_p_returnType"))
        ret.append(TextEdit("Return Description:","_p_returnDescription",True))
        return ret


    def icon(
        self
    ):
        return QIcon(":/socref_glsl/function.svg")


    def key(
        self
    ):
        ret = super().key()
        return ret+"("+self.__signature_()+")"


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "function"
        self._p_returnType = "void"
        self._p_returnDescription = ""


    def __signature_(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : string
                 The signature of this function used to generate its key. This
                 only includes its arguments.
        """
        return ",".join((type_.replace(" ","") for (name,type_,description) in self.arguments()))
