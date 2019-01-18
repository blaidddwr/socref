lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")
lessThan(QT_MINOR_VERSION,7): error("Requires Qt 5.7")

MAJOR_VERSION = 0
MINOR_VERSION = 6
REVISION = 0

QT += core gui widgets xml

CONFIG += c++11

TARGET = socref
TEMPLATE = app
VERSION = $${MAJOR_VERSION}.$${MINOR_VERSION}.$${REVISION}

LIBS += -lsocutil -laspell

DEFINES += QT_DEPRECATED_WARNINGS \
    MAJOR_VERSION=$${MAJOR_VERSION} \
    MINOR_VERSION=$${MINOR_VERSION} \
    REVISION=$${REVISION}

SOURCES += \
    main.cpp \
    project.cpp \
    abstractblock.cpp \
    gui_mainwindow.cpp \
    projectfactory.cpp \
    application.cpp \
    blockmodel.cpp \
    abstractparser.cpp \
    scanthread.cpp \
    basicblockfactory.cpp \
    basicblock.cpp \
    basicblock_edit.cpp \
    basicblock_view.cpp \
    dictionarymodel.cpp

HEADERS += \
    global.h \
    abstractprojectfactory.h \
    project.h \
    abstractblockfactory.h \
    abstractblock.h \
    projectfactory.h \
    application.h \
    blockmodel.h \
    abstractparser.h \
    abstractparserfactory.h \
    scanthread.h \
    dictionarymodel.h \
    basicblockfactory.h \
    basicblock.h \
    basicblock_edit.h \
    basicblock_view.h

include(Gui.pri)
include(CppQt.pri)
include(GLSL.pri)

RESOURCES += \
    resources.qrc

isEmpty(PREFIX) { PREFIX = /usr/local }
program.path = $${PREFIX}/bin
program.files = $${OUT_PWD}/$${TARGET}
INSTALLS += program
