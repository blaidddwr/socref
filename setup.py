import sys
from setuptools import setup
from setuptools import find_packages
try:
    from socref.Settings import Settings
except ImportError:
    print(
        "Failed loading package module. Did you run make before pip to generate resource modules?"
        ,file=sys.stderr
    )
    sys.exit(-1)
setup(
    name="socref"
    ,version=Settings.VERSION
    ,packages=find_packages()
    ,entry_points={"gui_scripts": ["socref = socref.__main__:main"]}
)
