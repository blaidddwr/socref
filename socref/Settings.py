"""
Contains the Settings class.
"""
from hunspell import HunSpell
from os.path import join as pathJoin




class Settings():
    """
    This is the settings class. It provides all global settings used by this
    application.
    """
    ORGANIZATION_NAME = "Galtwe Software"
    APPLICATION_NAME = "Socrates' Reference"
    VERSION = "0.9.0"
    DICTIONARY = "en_US"
    HUNSPELL_ROOT = "/usr/share/hunspell"
    speller = HunSpell(
        pathJoin(HUNSPELL_ROOT,DICTIONARY+".dic")
        ,pathJoin(HUNSPELL_ROOT,DICTIONARY+".aff")
    )
    LANGS = (
        ("C++","socref_cpp")
        ,("C++/Qt","socref_cppqt")
        ,("GLSL","socref_glsl")
        ,("Python","socref_python")
    )
    FILE_EXTENSION = "Socrates' Reference Project File (*.srp)"


    class BlockEditDock():
        """
        This is the block edit dock settings subclass. It provides all settings
        for this application's block edit dock.
        """
        MARGIN_LEFT = 0
        MARGIN_TOP = 16
        MARGIN_RIGHT = 0
        MARGIN_BOTTOM = 4


    class ProjectModel():
        """
        This is the project model settings subclass. It provides all settings
        for this application's project model.
        """
        COPY_TAG = "pysoref_copy"
        LANG_TAG = "language"
        NAME_TAG = "name"
        PARSE_PATH_TAG = "parse_path"
        PROJECT_TAG = "srp_project"
