lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")
greaterThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")
equals(QT_MAJOR_VERSION,5)
{
    lessThan(QT_MINOR_VERSION,7): error("Requires Qt 5.7 or greater")
}

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
    blockmodel.cpp \
    scanner.cpp \
    scanthread.cpp \
    dictionarymodel.cpp \
    projectfactory.cpp \
    project.cpp \
    application.cpp \
    main.cpp

HEADERS += \
    global.h \
    blockmodel.h \
    scanner.h \
    scanthread.h \
    dictionarymodel.h \
    projectfactory.h \
    project.h \
    application.h

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
