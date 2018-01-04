QT += core gui widgets

CONFIG += c++11

TARGET = socref
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    project.cpp \
    xmlelementparser.cpp \
    abstractblock.cpp \
    gui_mainwindow.cpp \
    projectfactory.cpp \
    exception.cpp \
    application.cpp \
    gui_projectdialog.cpp \
    blockmodel.cpp \
    cppqt_blockfactory.cpp \
    cppqt_namespace.cpp \
    gui_blockview.cpp \
    gui_abstractedit.cpp \
    cppqt_type.cpp \
    cppqt_edit_namespace.cpp \
    cppqt_view_namespace.cpp \
    cppqt_abstracttype.cpp \
    cppqt_abstractcontainer.cpp \
    cppqt_type_concrete.cpp \
    cppqt_definition.cpp \
    cppqt_type_template.cpp

HEADERS += \
    singleton.h \
    singletonfactory.h \
    exception.h \
    abstractprojectfactory.h \
    project.h \
    abstractblockfactory.h \
    xmlelementparser.h \
    abstractblock.h \
    gui_mainwindow.h \
    projectfactory.h \
    application.h \
    gui_projectdialog.h \
    blockmodel.h \
    cppqt_blockfactory.h \
    cppqt_namespace.h \
    gui_blockview.h \
    gui_abstractedit.h \
    cppqt_type.h \
    cppqt_edit_namespace.h \
    cppqt_view_namespace.h \
    cppqt_abstracttype.h \
    cppqt_abstractcontainer.h \
    cppqt_abstractdefinition.h \
    cppqt_type_concrete.h \
    cppqt_definition.h \
    cppqt_type_template.h

RESOURCES += \
    resources.qrc
