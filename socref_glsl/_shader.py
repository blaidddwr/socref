"""
Contains the Shader class.
"""
import html
from PySide2 import QtGui as qtg
from socref import public as scr
from . import settings
from ._base import Base








@scr.register("Shader")
class Shader(Base):
    """
    This is the shader block class. It implements the Socrates' Reference
    abstract block class. It represents a GLSL shader.
    """


    def __init__(
        self
        ):
        """
        Initializes a new shader block.
        """
        super().__init__()
        self._p_type = "Vertex"


    def build(
        self
        ,definition
        ,begin=""
        ):
        """
        Implements the socref_glsl.block.Base interface.

        Parameters
        ----------
        definition : object
                     See interface docs.
        begin : object
                See interface docs.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = definition.pop("header")
        ret += (
            self._buildChildren_(definition,begin)
            + [""]*settings.H1LINES
            + ["/*!"]
            + scr.wrapText(self._p_description,begin+" * ",columns=settings.COLUMNS)
            + [" */"]
            + ["void main()"]
            + definition["functions"].pop("main",["{"]) + ["}"]
        )
        return ret


    def buildList(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        return ("Variable","Structure","Function")


    def clearProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.clearProperties(self)
        self._p_type = "Vertex"


    def displayView(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        type_ = "<h2>Type</h2><p>"+html.escape(self._p_type)+"</p>"
        return Base.displayView(self)+type_


    def editDefinitions(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        ret = Base.editDefinitions(self)
        combo = scr.comboEdit("Type:","_p_type")
        scr.addComboSelect(combo,"Vertex",qtg.QIcon(":/glsl/vertex_shader.svg"))
        scr.addComboSelect(combo,"Fragment",qtg.QIcon(":/glsl/fragment_shader.svg"))
        scr.addComboSelect(combo,"Tesselation Control",qtg.QIcon(":/glsl/tesselation_shader.svg"))
        scr.addComboSelect(combo,"Tesselation Evaluation",qtg.QIcon(":/glsl/tesselation_shader.svg"))
        scr.addComboSelect(combo,"Geometry",qtg.QIcon(":/glsl/geometry_shader.svg"))
        scr.addComboSelect(combo,"Compute",qtg.QIcon(":/glsl/compute_shader.svg"))
        ret.append(combo)
        return ret


    def extension(
        self
        ):
        """
        Getter method.

        Returns
        -------
        ret0 : string
               A file extension based off this shader's type.
        """
        if self._p_type == "Vertex":
            return settings.VERTEX_EXTENSION
        elif self._p_type == "Fragment":
            return settings.FRAGMENT_EXTENSION
        elif self._p_type == "Tesselation Control":
            return settings.TESS_CTRL_EXTENSION
        elif self._p_type == "Tesselation Evaluation":
            return settings.TESS_EVAL_EXTENSION
        elif self._p_type == "Geometry":
            return settings.GEOMETRY_EXTENSION
        elif self._p_type == "Compute":
            return settings.COMPUTE_EXTENSION
        else:
            raise RuntimeError("Shader is invalid type.")


    def icon(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.

        Returns
        -------
        ret0 : object
               See interface docs.
        """
        if self._p_type == "Vertex":
            return qtg.QIcon(":/glsl/vertex_shader.svg")
        elif self._p_type == "Fragment":
            return qtg.QIcon(":/glsl/fragment_shader.svg")
        elif self._p_type.startswith("Tesselation"):
            return qtg.QIcon(":/glsl/tesselation_shader.svg")
        elif self._p_type == "Geometry":
            return qtg.QIcon(":/glsl/geometry_shader.svg")
        elif self._p_type == "Compute":
            return qtg.QIcon(":/glsl/compute_shader.svg")
        else:
            raise RuntimeError("Shader is invalid type.")


    def setDefaultProperties(
        self
        ):
        """
        Implements the socref.abstract.AbstractBlock interface.
        """
        Base.setDefaultProperties(self)
        self._p_name = "shader"
        self._p_type = "Vertex"
