
# Socrates Reference

This program acts as a reference tool for programming projects. This holds a reference to all source code objects. Each object in turn has comments added to them. This program is then capable of parsing the actual source code and adding the reference objects along with their comments to the source code. Any actual code within the methods being written are preserved that allows for editing of the source code as it is parsed.

## Supported Languages

The following languages are supported:

* C++/Qt
* GLSL

## Dependencies

This requires the Socrates Utilities library.

## Installation

This is a normal qt project using qmake. The PREFIX variable defines where the application is installed. The default PREFIX is /usr/bin.

```shell
cd build/release
qmake ../../src/SocRef.pro PREFIX=/usr/bin
make qmake_all
make
make qmake_all
make install
```
