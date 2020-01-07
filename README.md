
# Socrates Reference

This python GUI application provides a description reference to source code of a programming project. It provides the ability to parse the source code and add all descriptions it contains as comments in the code itself while preserving any code actually written. Any source code files that should exist but do not are automatically created.

By providing these set of tools it allows programmers to easily document their code with descriptive comments. The descriptions held in this application are spell checked and all formatting is done automatically when source code is parsed. This allows keeping all code fully commented very easy and encourages descriptive robustness of a project's comments.

References are maintained in this application based off objects associated with the programming language. For example in C++ there are objects for classes, functions, and variables. These objects are arranged within a tree like list within a project, usually following how the scoping of the language it is referencing works. For example in C++ objects are nested by namespaces and classes.

## Supported Languages

The following languages are supported:

* C++
* C++/Qt
* GLSL
* Python

## Dependencies

This requires Qt PySide2 and hunspell python packages. PySide2 is NOT included in the setup tools "install_requires" directive and will not be automatically installed when using pip. Hunspell is included and will be installed for you by pip.

## Installation

Commands must be done in the following order:

1. Clone the git repository.
2. Run make in the repository root folder.
3. Run pip install.

```shell
git clone ... socref
cd socref
make
pip install .
```

### Free Desktop

Run the following commands in the project's root folder to integrate this application in linux on KDE/Gnome.

```shell
cp socref.desktop ~/.local/share/applications
cp socref/data/application.svg ~/.local/share/icons/socref.svg
```
