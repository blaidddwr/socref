lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")
lessThan(QT_MINOR_VERSION,7): error("Requires Qt 5.7")

MAJOR_VERSION = 0
MINOR_VERSION = 0
REVISION = 9999

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
    exception.cpp \
    application.cpp \
    abstractparser.cpp \
    blockmodel.cpp \
    scanner.cpp \
    scanthread.cpp \
    dictionarymodel.cpp \
    projectfactory.cpp \
    project.cpp \
    main.cpp

HEADERS += \
    global.h \
    exception.h \
    application.h \
    abstractparser.h \
    abstractparserfactory.h \
    blockmodel.h \
    scanner.h \
    scanthread.h \
    dictionarymodel.h \
    projectfactory.h \
    project.h

include(Abstract.pri)
include(Gui.pri)
include(Basic.pri)
include(CppQt.pri)
include(GLSL.pri)

RESOURCES += \
    resources.qrc

isEmpty(PREFIX) { PREFIX = /usr/local }
program.path = $${PREFIX}/bin
program.files = $${OUT_PWD}/$${TARGET}
INSTALLS += program
