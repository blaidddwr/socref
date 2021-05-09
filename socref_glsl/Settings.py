"""
Contains the Settings class.
"""




class Settings():
    """
    This is the settings class. It provides all global setting values used by
    the GLSL language implementation.
    """
    H1 = 2
    H2 = 1
    INDENT = "    "
    COLS = 80
    EXT = {
        "Vertex": ".v"
        ,"Fragment": ".f"
        ,"Tesselation Control": ".tc"
        ,"Tesselation Evaluation": ".te"
        ,"Geometry": ".g"
        ,"Compute": ".c"
    }
