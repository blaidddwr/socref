
# Socrates Reference

This is a python GUI application that provides tools for designing programming architecture in a project document and generating source code for its design architecture documents. It places emphasis on documentation of all design components of a programming project, outputting that documentation as neatly organized comments in the generated code. This application also parses any existing code, saving any implementation lines of code and adding them back into the code generated from its design reference document.

This application structures its design documents using objects that can have any number of children, creating a tree like structure. Each project document must begin with a root object. Objects represent specific objects of a programming language, such as functions or variables. Because all programming languages follow a hierarchal structure all objects can be structured in a tree like structure. The root object’s representation of a project document greatly depends on the specific language. For example in C++ it would represent the global name space.

Almost all design objects of a programming project’s architecture in this application provide descriptions. There are very few exemptions where an object in a design document would not provide any description. This, combined with the graphical design of objects themselves, provides the ability to easily understand a program’s code and its architecture.

This application’s generated code only provides the overall structure of a programming project and not the actual code to implement its algorithms and functions. Because of this it parses any code present in a program’s source code, saving it and outputting it in its generated code that overwrites the original. In this fashion this application can easily generate code to existing projects without losing any implementation code actually written. The specific lines that are parsed are highly dependent on the language of the program’s code. For example in C++ the majority of code is parsed from within functions.

## Supported Languages

The following languages are supported:

* C++
* C++/Qt
* GLSL
* Python

## Dependencies

The following python packages are required:

* PySide6
* hunspell

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

The make process uses Qt's RCC command to compile resources into python scripts. Using Qt version 6 is required. The RCC make variable can be used to override the default if it is not Qt version 6.

```
make RCC=/path/to/qt6/rcc
```

### Free Desktop

Run the following commands in the project's root folder to integrate this application in Linux on KDE/Gnome.

```shell
cp socref.desktop ~/.local/share/applications
cp socref/data/application.svg ~/.local/share/icons/socref.svg
```
