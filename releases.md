
# Releases

Description of all releases with most recent on the top.

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
* Added C++/Qt struct and union blocks.
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
* Improved C++/Qt parsing of initializers.

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
