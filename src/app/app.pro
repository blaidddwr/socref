TEMPLATE = app
QT += core gui widgets
CONFIG += c++17
INCLUDEPATH += ../corelib ../cpplib ../cppqtlib
LIBS += -L$$OUT_PWD/../libs -lcppqt -lcpp -lcore -lhunspell
PRE_TARGETDEPS += \
    $$OUT_PWD/../libs/libcore.a \
    $$OUT_PWD/../libs/libcpp.a \
    $$OUT_PWD/../libs/libcppqt.a
SOURCES += main.cpp
TARGET = socref
