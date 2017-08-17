QT += core gui widgets

CONFIG += c++11

TARGET = socref
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    project.cpp \
    projectregistry.cpp \
    xmlelementparser.cpp

HEADERS += \
    mainwindow.h \
    singleton.h \
    singletonfactory.h \
    exception.h \
    abstractprojectfactory.h \
    project.h \
    abstractblockfactory.h \
    projectregistry.h \
    xmlelementparser.h

#AbstractBlock
#Gui::AbstractView
#Gui::AbstractEdit

#BlockModel (QAbstractItemModel)
#Gui::BlockView (QTreeView)


#....
#CppQt::Namespace::Block
#CppQt::Namespace::View
#CppQt::Namespace::Edit
