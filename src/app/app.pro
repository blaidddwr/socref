TEMPLATE = app
QT += core gui widgets testlib
CONFIG += c++17
INCLUDEPATH += ../lib
LIBS += -lhunspell -L$$OUT_PWD/../lib -lsocref
SOURCES += main.cpp
