
# Coding Standard

This describes the coding standard used by this application. Its primary purpose is to make it as identical as possible between python and C++, with Qt as a shared platform. Any differences between the two languages are indicated in different subsections. The built in formatting this application produces is also described.

File structure is defined in regard to how directories and files should relate to the code structure.

## Whitespace

Trailing whitespace before the newline character of each line is not allowed. Four spaces are used for indentation.

## Blank Lines

Two blank lines are used as margin for each function and method definition. Margins are not added at the end of a file.

### Python

Four blank lines are used as margin for each class definition.

### C++

Four blank lines are used as margin for each class declaration.

Two blank lines are used as margin for each function and method declaration, class access line, enumeration, and variable declaration.

## Namespaces

Pascal case is used for names. Names starts with a letter and do not contain underscores.

Namespaces are represented as a directory with the same name in the file system.

### Python

Packages are considered namespaces.

Since directories are strictly defined as packages, the usual init module exists within each package directory. The special main module is also present if it is the root package of an application. All singleton class instances are defined in the special init module.

### C++

A header and source file that provides forward class declarations for all classes declared within a namespace, exists in each namespace directory. The name of these files are '\_\_init\_\_'.

## Special Entry Point

This only applies to applications.

### Python

The special main module provides an entry point for an application in the root package. A global main function, to be defined as the entry point of an application, is allowed in this main module. The required if statement is also allowed in this main module.

### C++

The special global main function of a C++ application is defined in a source file called main. This is the only global non-method function that is allowed.

## Classes

Pascal case is used for names. Names starts with a letter and do not contain underscores.

Nested classes are not allowed.

## Python

Each class is defined in its own separate module file. The module file name is the class name. Each class module is imported into the package namespace through the special init module.

## C++

Each class is defined in its own separate header and source file. The header and source file names are the class name.

## Globals

Global objects within namespaces are dangerous and never used if possible.

### Python

Only class definitions and class singleton instances are named globally within any package. The one special case exception is the special main module and its main entry point.

### C++

Because C++ is a real language, absolutely nothing is global.

## Import/Include

### Python

When importing from other modules and packages, the specific objects that are used are imported. If there are naming conflicts an object can be named something else. Only one object can be imported for each import line used. The special import everything character can be used for class modules since they only contain a single class definition.

```
from one import oneSingleton
from . import one as localOneSingleton
from .FooClass import *
```

The one exception to importing objects and not modules or packages is if there is a circular dependency between two classes that cannot be resolved. In this case one of the class modules is allowed to import the module or package itself.

### C++

All include and using statements are at the beginning of any source file, in that order. Using statements are not used in header files. When including type entities from other namespaces they are pulled into the local namespace. If there are naming conflicts a type can be named something else.

```
#include "../Foo/One.h"
#include "../Bar/One.h"
using Foo::Bar as FooOne;
using Bar::One as BarOne;
```

## Methods

Functions are always defined as a method of a class.

Lower case camel case is used for names.

Function definition lines are separated into multiple lines for each argument defined. If no arguments are defined then the definition is a single line. Commas used to separate multiple arguments are added to the beginning of each line, starting with the second argument.

### Python

Protected methods start and end with one underscore.

```
class Foo():
    def protectedMethod(
        self
    ):
        pass
```

Private methods start with two underscores and end with one.

```
class Foo():
    def privateMethod(
        self
    ):
        pass
```

Blank lines are not used correctly in this example as it is contrived.

### C++

The special case main function is allowed to be global and not a method.

The opening and closing brackets of a function are on their own separate lines.

```
void doSomething(
    int a
    ,int b
)
{
}
```

## Comments

### Classes

### Functions

### C++

#### Enumerations

#### Variables
