QT += core gui widgets xml

CONFIG += c++11

TARGET = socref
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    project.cpp \
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
    cppqt_gui_typelistdialog.cpp \
    cppqt_gui_typedialog.cpp

HEADERS += \
    singleton.h \
    singletonfactory.h \
    exception.h \
    abstractprojectfactory.h \
    project.h \
    abstractblockfactory.h \
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
    classes.h \
    cppqt_classes.h \
    gui_classes.h \
    cppqt_edit_classes.h \
    cppqt_view_classes.h \
    cppqt_gui_typelistdialog.h \
    cppqt_gui_typedialog.h

RESOURCES += \
    resources.qrc
