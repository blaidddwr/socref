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
    cppqt_namespaceedit.cpp \
    cppqt_namespaceview.cpp

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
    cppqt_namespaceedit.h \
    cppqt_namespaceview.h

#AbstractParser
#Gui::AbstractView
#Gui::AbstractEdit

#BlockModel (QAbstractItemModel)
#Gui::BlockView (QTreeView)


#....
#CppQt::Namespace::Block
#CppQt::Namespace::View
#CppQt::Namespace::Edit
