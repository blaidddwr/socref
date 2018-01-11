QT += core gui widgets xml

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
    cppqt_edit_namespace.cpp \
    cppqt_view_namespace.cpp \
    cppqt_abstracttype.cpp \
    cppqt_abstractcontainer.cpp \
    cppqt_type_concrete.cpp \
    cppqt_definition.cpp \
    cppqt_type_template.cpp \
    cppqt_typefactory.cpp \
    cppqt_abstractdefinition.cpp \
    cppqt_view_definition.cpp \
    cppqt_edit_definition.cpp

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
    cppqt_edit_namespace.h \
    cppqt_view_namespace.h \
    cppqt_abstracttype.h \
    cppqt_abstractcontainer.h \
    cppqt_abstractdefinition.h \
    cppqt_type_concrete.h \
    cppqt_definition.h \
    cppqt_type_template.h \
    cppqt_typefactory.h \
    classes.h \
    cppqt_classes.h \
    gui_classes.h \
    cppqt_type_classes.h \
    cppqt_edit_classes.h \
    cppqt_view_classes.h \
    cppqt_view_definition.h \
    cppqt_edit_definition.h

RESOURCES += \
    resources.qrc
