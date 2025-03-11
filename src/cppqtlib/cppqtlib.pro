TEMPLATE = lib
QT += core gui widgets
CONFIG += c++17 staticlib
INCLUDEPATH += ../corelib ../cpplib
SOURCES += \
    CppQtBlockFunction.cpp \
    CppQtLanguage.cpp
HEADERS += \
    CppQt.h \
    CppQtBlock.h \
    CppQtBlockFunction.h \
    CppQtLanguage.h
RESOURCES += cppqt.qrc
DESTDIR = $$OUT_PWD/../libs
TARGET = cppqt
