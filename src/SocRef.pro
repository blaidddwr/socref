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
    dictionarymodel.cpp \
    gui_projectdialog.cpp \
    gui_blockview.cpp \
    gui_abstractedit.cpp \
    gui_textedit.cpp \
    gui_textedit_highlighter.cpp \
    gui_textedit_dialog.cpp \
    gui_textdialog.cpp \
    gui_scandialog.cpp \
    gui_persistentdialog.cpp \
    gui_listedit.cpp \
    gui_listedit_model.cpp \
    gui_dictionarydialog.cpp \
    gui_typeselection.cpp

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
    basicblock_view.h \
    gui.h \
    gui_mainwindow.h \
    gui_projectdialog.h \
    gui_blockview.h \
    gui_abstractedit.h \
    gui_textedit.h \
    gui_textedit_highlighter.h \
    gui_textedit_dialog.h \
    gui_textdialog.h \
    gui_scandialog.h \
    gui_persistentdialog.h \
    gui_listedit.h \
    gui_listedit_model.h \
    gui_dictionarydialog.h \
    gui_typeselection.h

include(CppQt.pri)

RESOURCES += \
    resources.qrc

isEmpty(PREFIX) { PREFIX = /usr/local }
program.path = $${PREFIX}/bin
program.files = $${OUT_PWD}/$${TARGET}
INSTALLS += program
