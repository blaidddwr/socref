"""
Installs socref. For the resource files to be properly installed make MUST be run before running pip
install.
"""
import sys
from setuptools import setup
from setuptools import find_packages
try:
    from socref import settings
except ImportError:
    print(
        "Failed loading package module. Did you run make before pip to generate resource modules?"
        ,file=sys.stderr
    )
    sys.exit(-1)

setup(
    name="socref"
    ,version=settings.VERSION
    ,packages=find_packages()
    ,entry_points={"gui_scripts": ["socref = socref.__main__:main"]}
)
