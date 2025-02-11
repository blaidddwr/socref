TEMPLATE = app
QT += core gui widgets testlib
CONFIG += c++17
INCLUDEPATH += ../cppqtlib ../cpplib ../corelib
LIBS += \
    -lhunspell \
    -L$$OUT_PWD/../cpplib -lcpp \
    -L$$OUT_PWD/../cppqtlib -lcppqt \
    -L$$OUT_PWD/../corelib -lcore
PRE_TARGETDEPS += \
    $$OUT_PWD/../cpplib/libcpp.a \
    $$OUT_PWD/../cppqtlib/libcppqt.a \
    $$OUT_PWD/../corelib/libcore.a
SOURCES += main.cpp
TARGET = socref
