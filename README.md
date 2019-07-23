
# Socrates Reference

This application provides a description reference to source code of a
programming project. This application also provides the ability to parse the
actual source code and add all descriptions it contains as comments in the code
itself while preserving any code actually written.

By providing these set of tools it allows programmers to easily document their
code with descriptive comments. The descriptions held in this application are
spell checked and all formatting is done automatically when source code is
parsed. This allows keeping all code fully commented very easy and encourages
descriptive robustness of a project's comments.

References are maintained in this application based off objects associated with
the programming language. For example in C++ there are objects for classes,
functions, and variables. These objects are arranged within a tree like list
within this program, usually following how the scoping of the language it is
referencing works. For example in C++ objects are nested by namespaces and
classes.

## Supported Languages

The following languages are supported:

* C++/Qt
* GLSL

## Dependencies

This requires the Socrates Utility library.

## Installation

This is a normal qt project using qmake. The PREFIX variable defines where the
application is installed. The default PREFIX is /usr/bin.

```shell
cd build/release
qmake ../../src/SocRef.pro PREFIX=/usr/bin
make qmake_all
make
make qmake_all
make install
```
