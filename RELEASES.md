
# Releases

Description of all releases with most recent on the top.

## NEXT VERSION

* Fixed C++ function bugs.
* Added C++ post line scanning for custom code after declarations.
* Added option to make C++ enumerations a class.
* Updated XML output of reference files to have deterministic output.
* Added C++ function signature recognition of constant.
* Completely changed file format for C++ projects to be flat files with no directories.
* Added C++ scanning of forward class declarations in header's macro section.

## Version 0.9.0

* Changed file extension for projects to SRP.
* Huge backwards compatibility breaking changes to core application.
* Added new reader and writer interfaces to core application for language implementations.
* Updated parser interface to use new reader and writer interfaces.
* Added helper classes for edit definitions, rich text output, and code output.
* Changed location and virtual folder names of Qt resource data.
* Completely revamped coding standards for all languages.
* Massive update for all languages using the new reader/writer system.
* Greatly refined code structure of languages, adding base classes where appropriate.

## Version 0.8.6

* Fixed core spell checker bug.
* Updated python to more follow the mainline standard.
* Added custom code line parsing for python classes.
* Refined custom code line parsing for python modules.

## Version 0.8.5

* Fixed GLSL bugs.
* Modified GLSL file extensions.
* Added function signature support to GLSL.
* Modified function output and object comments of python parsing.
* Added scripting support, inline classes, and enumerations to python.
* Modified default column size of comments to 80 for all languages.
* Fixed GUI text dialog bug.

## Version 0.8.4

* Modified python parsing so each class is contained in its own special module.
* Fixed core bugs.
* Fixed python parsing bug.

## Version 0.8.3

* Fixed core bugs.
* Fixed C++ parsing bugs.
* Modified python parsing output for function doc strings to follow numpy standard.

## Version 0.8.2

* Fixed C++/Qt parsing bugs.
* Removed support for all C++ implicit type conversion operators.

## Version 0.8.1

* Fixed C++/Qt bugs.
* Added basic check in setup tools script.
* Added to C++ parsing.
* Added command line argument processing for opening a project.

## Version 0.8.0

* Complete overhaul of entire program to python.
* Still uses Qt through the PySide2 python interface.
* Added python project language.
* Complete overhaul of the entire GUI.
* Added docker view and edit widgets for blocks.
* Added toolbar for common actions along with free desktop icons.
* Changed file extension for projects to SCP.
* Drastically modified parser output for C++ files.
* Added additional C++ template argument support.
* Inline comment support has been removed.

## Version 0.7.2

* Fixed project relative path bugs.
* Fixed C++/Qt parser bug.
* Trimmed notes.

## Version 0.7.1

* Optimized scanner for all project types.
* Improved project error handling.
* Fixed C++/Qt declaration block bugs.
* Trimmed project by removing file watcher that never worked anyway.
* Improved about dialog.
* Moved notes.

## Version 0.7.0

* Modified internal debugging Qt standards.
* Overhauled parsing system.
* Modified C++/Qt parsing so only root classes have separate source files.
* Added C++/Qt structure and union blocks.
* Modified C++/Qt declarations.
* Fixed display bug.

## Version 0.6.0

* Added new basic block layer above the abstract layer.
* Modified C++/Qt to use the new basic block layer.
* Added GLSL project type for commenting OpenGL shader code.

## Version 0.5.1

* Improved C++/Qt types.
* Added C++/Qt thread local property for variables.
* Fixed C++/Qt parsing bugs.

## Version 0.5.0

* Overhauled block System.
* Trimmed C++/Qt by removing type and type list blocks.
* Modified C++/Qt parsing.
* Modified C++/Qt function view.
* Added C++/Qt deleted property functions.
* Added custom spell checking words system used on a per project basis.
* Improved open project file dialog.

## Version 0.4.6

* Fixed edit->cut block bug.
* Fixed block view bugs.
* Fixed C++/Qt bug.

## Version 0.4.5

* Fixed C++/Qt parsing bug.
* Improved C++/Qt views.
* Modified C++/Qt type system to allow lists within lists.

## Version 0.4.4

* Improved C++/Qt settings dialog by adding max columns field.
* Fixed C++/Qt bugs.
* Improved C++/Qt type combo boxes.

## Version 0.4.3

* Added C++/Qt type list and type to the main block system.
* Improved C++/Qt enumerations tree view.
* Improved C++/Qt declarations syntax checks.

## Version 0.4.2

* Improved Views to update themselves better.
* Fixed C++/Qt viewing bugs.
* Modified C++/Qt function tree view properties.

## Version 0.4.1

* Fixed C++/Qt parsing bugs.
* Fixed C++/Qt viewing bugs.
* Fixed object reference bug.

## Version 0.4.0

* Improved edit block dialog GUI to properly handle failures.
* Added project settings menu that opens a settings dialog project types.
* Added help menu.
* Fixed C++/Qt parsing bugs.
* Modified C++/Qt parent block to allow scope.
* Added C++/Qt using and friend declarations.
* Added C++/Qt mutable and default properties for variables and functions.
* Modified C++/Qt display of certain blocks so they are more readable.
* Improved C++/Qt edit dialog GUI to remember its geometry.

## Version 0.3.8

* Modified block saving and loading to and from XML.
* Fixed block index bug.
* Fixed C++/Qt bugs.
* Improved C++/Qt parsing of private template methods.

## Version 0.3.7

* Fixed C++/Qt parser bugs.

## Version 0.3.6

* Fixed C++/Qt bugs.

## Version 0.3.5

* Improved C++/Qt support by adding language syntax checks.
* Improved C++/Qt appearance in block editor.
* Improved C++/Qt parsing of constructors.

## Version 0.3.4

* Fixed many C++/Qt bugs.
* Fixed scanning bug.

## Version 0.3.3

* Fixed project class bug.
* Fixed C++/Qt parser bug.

## Version 0.3.2

* Fixed C++/Qt parser bugs.
* Improved C++/Qt parsing output.

## Version 0.3.1

* Fixed C++/Qt bug.
* Improved C++/Qt support with block editing and scanning.

## Version 0.3.0

* Added support for scanning directories and parsing code files.
* Added C++/Qt support to scanning and parsing code.

## Version 0.2.0

* Added tree view block editor that supports programming blocks.
* Added support for C++/Qt using the new tree view block editor.
* Improved GUI with various minor changes to make the interface better and
  easier to use.

## Version 0.1.0

* Added main window GUI.
* Added project class.
