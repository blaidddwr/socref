"""
Contains the ShaderBlock class.
"""
from .ProgramBlock import *
from PySide2 import QtGui as qtg
from socref import block
from socref.Edit.ComboEdit import *




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
        self._p_type = "Vertex"


    def buildList(
        self
    ):
        return ("Variable","Structure")


    def clearProperties(
        self
    ):
        super().clearProperties()
        self._p_type = "Vertex"


    def displayView(
        self
    ):
        rt = super().displayView()
        rt.addHeader("Type",1)
        rt.addText(self._p_type)
        return rt


    def editDefinitions(
        self
    ):
        edits = super().editDefinitions()
        edits.append(
            ComboEdit(
                "Type:"
                ,"_p_type"
                ,(
                    (qtg.QIcon(":/socref_glsl/vertex_shader.svg"),"Vertex")
                    ,(qtg.QIcon(":/socref_glsl/fragment_shader.svg"),"Fragment")
                    ,(qtg.QIcon(":/socref_glsl/tesselation_ctrl_shader.svg"),"Tesselation Control")
                    ,(qtg.QIcon(":/socref_glsl/tesselation_eval_shader.svg"),"Tesselation Evaluation")
                    ,(qtg.QIcon(":/socref_glsl/geometry_shader.svg"),"Geometry")
                    ,(qtg.QIcon(":/socref_glsl/compute_shader.svg"),"Compute")
                )
            )
        )
        return edits


    def icon(
        self
    ):
        if self._p_type == "Vertex":
            return qtg.QIcon(":/socref_glsl/vertex_shader.svg")
        elif self._p_type == "Fragment":
            return qtg.QIcon(":/socref_glsl/fragment_shader.svg")
        elif self._p_type == "Tesselation Control":
            return qtg.QIcon(":/socref_glsl/tesselation_ctrl_shader.svg")
        elif self._p_type == "Tesselation Evaluation":
            return qtg.QIcon(":/socref_glsl/tesselation_eval_shader.svg")
        elif self._p_type == "Geometry":
            return qtg.QIcon(":/socref_glsl/geometry_shader.svg")
        elif self._p_type == "Compute":
            return qtg.QIcon(":/socref_glsl/compute_shader.svg")
        else:
            raise RuntimeError("Shader is invalid type.")


    def setDefaultProperties(
        self
    ):
        super().setDefaultProperties()
        self._p_name = "shader"
        self._p_type = "Vertex"
