QT += core gui widgets xml

CONFIG += c++11

TARGET = socref
TEMPLATE = app

INCLUDEPATH += ../../socerr/src
LIBS += -L../../socerr/build -lsocerr

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    project.cpp \
    abstractblock.cpp \
    gui_mainwindow.cpp \
    projectfactory.cpp \
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
    cppqt_gui_typedialog.cpp \
    cppqt_gui_typecombobox.cpp \
    cppqt_variable.cpp \
    cppqt_base.cpp \
    domelementreader.cpp \
    cppqt_edit_base.cpp \
    cppqt_edit_variable.cpp

HEADERS += \
    singleton.h \
    singletonfactory.h \
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
    cppqt_gui_typelistdialog.h \
    cppqt_gui_typedialog.h \
    cppqt_gui_typecombobox.h \
    cppqt_variable.h \
    cppqt_base.h \
    global.h \
    gui.h \
    cppqt.h \
    cppqt_edit.h \
    cppqt_gui.h \
    cppqt_view.h \
    cppqt_edit_base.h \
    cppqt_edit_variable.h

RESOURCES += \
    resources.qrc
