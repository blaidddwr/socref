
# Releases

Description of all releases with most recent on the top.

## Version 0.7.0

* Completely revamped debugging in internal code to follow Qt standards instead of using exceptions everywhere.
* Created massive overhaul of the parsing system for projects. Most of this is not visible to the user except for some minor things.
    * When scanning a project the progress bar is now displayed inside the main window on the status bar on the bottom.
    * Parsing C++/Qt classes nested within other classes are now all included in a single header and/or source file. This means only root classes that are part of a namespace have their own separate file with all nested classes defined within them.
* Added to C++/Qt project type:
    * Added struct and union blocks.
    * Added ability to add declarations outside any access block of a class without a semicolon, to be used for things like Q_PROPERTY.
* Fixed display bug for basic block view paragraphs where HTML characters were not properly escaped.

## Version 0.6.0

* Created massive overhaul of block system to add a new generic layer above the abstract layer.
    * The C++/Qt project type was completely overhauled to use this new layer.
        * Type lists have been removed. Types are now manually entered as a string for any relevant block type.
        * The display for various block types has been changed.
        * Slots, signals, constructors, and desctructors have all been merged into a single function block type.
* Added new GLSL project type for commenting OpenGL shader code.

## Version 0.5.1

* C++/Qt types can now contain "..." for variadic template arguments.
* Added the thread local property for C++/Qt variables.
* Fixed the absence of parsing output for using and friend declarations in C++/Qt.

## Version 0.5.0

* Massive internal overhaul of block code and basic structure. This radically changed the internal operations of this program and will show up in multiple small changes to the operation of this program. This should not change anything in a major way.
* Changed C++/Qt parsing:
    * Function operation comments are now inserted as inline comments inside their respective function.
    * Numerous minor changes throughout C++/Qt types as a result of the internal overhaul.
    * The special main function and its file is now parsed.
    * Fixed issue where parsing initializer lines of constructors with noexcept failed.
* The C++/Qt function view now only reports the total number of operations.
* Added the deleted property to C++/Qt functions.
* Added custom spell checking words to be added on a per project basis.
* Open project file dialog now persistently rememebers the last directory a project was opened.

## Version 0.4.6

* Fixed segmentation fault bug caused by cutting blocks.
* Fixed minor bugs with the block view not updating its current index or context menu.
* Fixed C++/Qt bug that caused any project with operator blocks to throw an error when being opened.

## Version 0.4.5

* Fixed C++/Qt parsing bug that did not properly recognize closing brackets with a semicolon.
* Added additional formatting to C++/Qt fields.
    * Added multiple paragraph support to description fields.
    * Added bold markers to function steps of operations field.
* Changed C++/Qt type system to allow lists within lists, along with changing the GUI which edit types to work like a menu instead of a combo box allowing for lists within lists.

## Version 0.4.4

* Added C++/Qt settings for max columns used for limiting the column length of comments and variable initializers.
* Fixed C++/Qt bugs:
    * The explicit property of functions are now correctly copied.
    * Template functions nested within a namespace are now correctly parsed.
* C++/Qt type combo boxes are now populated with all type lists above the block being edited.

## Version 0.4.3

* C++/Qt types are now visisble in the main block system as two new block types.
    * Added C++/Qt type list block which can hold a list of types.
    * Added C++/Qt type block which defines a specific type.
* C++/Qt enumerations now only display their name and a [C] flag if it is classed.
* C++/Qt declarations now checks and enforces correct syntax; as a result certain using declarations not allowed.

## Version 0.4.2

* Views now update themselves if their edit dialog has their apply button clicked.
* Fixed C++/Qt viewing bugs.
    * Enumerations and their values now display their descriptions.
    * Function views no longer displays an empty return type on constructors/desctructors.
* All C++/Qt function properties are now displayed the same way in the tree view; as single letter flags enclosed by square brackets.

## Version 0.4.1

* Fixed C++/Qt parsing bugs.
    * Default using namespace line for source files now correctly includes all parent namespaces.
    * Namespace descriptions are not written to global header files, excluding the global namespace.
* Fixed C++/Qt viewing bugs.
    * Function views now display return variable unless the type is void.
    * Function views now correctly display return types that are templates.
* Fixed segmentation fault caused using a reference after the object is deleted.

## Version 0.4.0

* Edit block dialogs now properly handle failures, informing the user what went wrong.
* Added Settings menu that opens a settings dialog for all project types.
    * Implemented settings dialog for C++/Qt.
* Added help menu.
    * Added about action for help menu that displays a simple about dialog for the program.
* Fixed C++/Qt parsing bugs.
    * Comments that contain parenthesis are no longer confused as function headers.
    * Templated classes are no longer forward declared in global header files.
* Complete overhaul of C++/Qt parents; now scope can be added to the class name.
* Added C++/Qt using and friend declarations.
* Added C++/Qt mutable property for class variables.
* Added C++/Qt default property for constructors, destructors, and operators.
* Changed C++/Qt display of certain blocks.
    * Changed display of classes.
        * Tree view name is now only its name and special characters.
        * Added inheritance section to its detailed view.
    * Changed display of functions.
        * Tree view name is now only its name and special characters.
        * Arguments list is now on top of its detailed view.
        * Modified appearance of operations in its detailed view to look better.
    * Changed display of variables.
        * Tree view name is now only its name and special characters.
        * Added description of its type to the top of its detailed view.
* All C++/Qt edit dialogs now remember their geometry.

## Version 0.3.8

* Overhauled how blocks are saved as XML to no longer use attributes heavily so the save file no longer randomly changes each time it is saved and no changes were made.
* Fixed segmentation fault bug caused by mishandling indexes when removing a block from the model.
* Fixed C++/Qt bugs:
    * Virtual and template functions can now coexist in a class.
    * Forward class declarations no longer outputed to common header files when parsed.
* C++/Qt private template methods now have their definitions in the source file instread of the header when parsed.

## Version 0.3.7

* Fixed C++/Qt parser bug where additional #endif was added to the header of common files each time it was scanned.
* Fixed C++/Qt parser bug of omission where nested classes would not have their forward declaration written to the header of their parent class.

## Version 0.3.6

* Fixed C++/Qt bug where the inherited classes in a class declaration would not output their template arguments when parsing headers.
* Fixed a C++/Qt type bug you the user was prevented from making types if the scope contained template arguments.

## Version 0.3.5

* Added multiple checks to C++/Qt for templates, virtual functions, and Qt Objects such that they are mutually exclusive.
* Changed appearance of C++/Qt blocks to be more compact and clear, primarily changing variables and functions.
* Added ability in C++/Qt parsing to write variable initializers with very long lists on multiple lines.

## Version 0.3.4

* Fixed numerous C++/Qt bugs:
    * Fixed numerous parsing bugs:
        * Destructor would be incorrectly matched with a blank constructor.
        * Spacing of the footer and source and header files were incorrect.
        * Normal variables part of a class would not show their initializers in the header file.
        * Enumerations would not write out a final semicolon required after their declaration.
    * Fixed a edit dialog box for constructors where the explicit checkbox was blank with no label to identify it.
* Fixed scanning bug where a dialog would execute outside of the main GUI thread if an exception was thrown inside the scanning thread.

## Version 0.3.3

* Fixed bug where project would not become modified when blocks are moved.
* Fixed C++/Qt parser bug where constructor, destructor, or operators would match any function causing undefined behavior in matching code to function blocks.

## Version 0.3.2

* Fixed C++/Qt parser bugs:
    * Function defintions were not being matched thereby discarding all code within the function.
    * Signal comments were not writing out argument comments.
* Added ability to write out inheritance for C++/Qt classes when parsing code.

## Version 0.3.1

* Fixed bug that prevented editing C++/Qt variables whose parents were specialized functions.
* C++/Qt source parser now additional outputs:
    * Default header include directive at beginning of file.
    * Comments now prepended to variable definitions.
* Fixed C++/Qt header parser output for functions; it now correctly outputs static property.
* Changed C++/Qt parser comments to use /\*! \*/ now instead of ///.
* Added explicit property for C++/Qt constructor type.
* Added ability to use C++/Qt move operator && when adding types.

## Version 0.3.0

* Added support for scanning directories and parsing code files. This makes the code a slave to its documentation because this program produces the basic skeleton code along with all comments and changes it based off what is in the program's view.
* Added C++/Qt support to scanning and parsing code.

## Version 0.2.0

* Moved settings->project action to file->properties.
* Added support for tree view editing of programming blocks.
* Programming blocks are saved/open to/from project files.
* Added support for C++/Qt using the new tree view editor.
* C++/Qt support includes: namespaces, classes, enumerations, functions, variables, operator overloading, and signals/slots.
* Other various minor GUI changes to make the interface better and easier to use.

## Version 0.1.0

* Basic GUI window that can hold a basic project.
* Basic project contains name, type, scan directory and filters.
