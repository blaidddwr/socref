"""
Installs socref. For the resource files to be properly installed make MUST be run before running pip
install.
"""
from setuptools import setup
from setuptools import find_packages

setup(
    name="socref"
    ,version="0.0.9999"
    ,packages=find_packages()
    ,install_requires=["hunspell"]
    ,entry_points={"gui_scripts": ["socref = socref.__main__:main"]}
)
