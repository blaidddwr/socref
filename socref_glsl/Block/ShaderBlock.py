"""
Contains the ShaderBlock class.
"""
from .ProgramBlock import ProgramBlock
from PySide6.QtGui import QIcon
from socref import block
from socref.Edit.ComboEdit import ComboEdit
from socref.Edit.LineEdit import LineEdit




@block("Shader")
class ShaderBlock(ProgramBlock):
    """
    This is the shader block class. It implements the Socrates' Reference
    abstract block class. It represents a GLSL shader.
    """


    def __init__(
        self
    ):
        super().__init__()
        self._p_version = "330 core"
        self._p_type = "Vertex"


    def buildList(
        self
    ):
        return ("Variable","Structure","Function")


    def clearProperties(
        self
    ):
        super().clearProperties()
        self._p_version = "330 core"
        self._p_type = "Vertex"


    def displayView(
        self
    ):
        ret = super().displayView()
        ret.addHeader("Version",1)
        ret.addText(self._p_version)
        ret.addHeader("Type",1)
        ret.addText(self._p_type)
        return ret


    def editDefinitions(
        self
    ):
        ret = super().editDefinitions()
        ret.append(LineEdit("Version:","_p_version"))
        ret.append(
            ComboEdit(
                "Type:"
                ,"_p_type"
                ,(
                    (QIcon(":/socref_glsl/vertex_shader.svg"),"Vertex")
                    ,(QIcon(":/socref_glsl/fragment_shader.svg"),"Fragment")
                    ,(QIcon(":/socref_glsl/tesselation_ctrl_shader.svg"),"Tesselation Control")
                    ,(QIcon(":/socref_glsl/tesselation_eval_shader.svg"),"Tesselation Evaluation")
                    ,(QIcon(":/socref_glsl/geometry_shader.svg"),"Geometry")
                    ,(QIcon(":/socref_glsl/compute_shader.svg"),"Compute")
                )
            )
        )
        return ret


    def icon(
        self
    ):
        if self._p_type == "Vertex":
            return QIcon(":/socref_glsl/vertex_shader.svg")
        elif self._p_type == "Fragment":
            return QIcon(":/socref_glsl/fragment_shader.svg")
        elif self._p_type == "Tesselation Control":
            return QIcon(":/socref_glsl/tesselation_ctrl_shader.svg")
        elif self._p_type == "Tesselation Evaluation":
            return QIcon(":/socref_glsl/tesselation_eval_shader.svg")
        elif self._p_type == "Geometry":
            return QIcon(":/socref_glsl/geometry_shader.svg")
        elif self._p_type == "Compute":
            return QIcon(":/socref_glsl/compute_shader.svg")
        else:
            raise RuntimeError("Shader is invalid type.")


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "shader"
        self._p_version = "330 core"
        self._p_type = "Vertex"
