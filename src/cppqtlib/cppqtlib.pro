TEMPLATE = lib
QT += core gui widgets testlib
CONFIG += c++17 staticlib
INCLUDEPATH += ../corelib ../cpplib
LIBS += -lhunspell
SOURCES += \
    CppQtBlockFunction.cpp \
    CppQtLanguage.cpp
HEADERS += \
    CppQt.h \
    CppQtBlock.h \
    CppQtBlockFunction.h \
    CppQtLanguage.h
RESOURCES += cppqt.qrc
TARGET = cppqt
