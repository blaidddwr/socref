QT += core gui widgets

CONFIG += c++11

TARGET = socref
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    abstractprojectfactory.cpp \
    projectmanager.cpp \
    projectpointer.cpp \
    project.cpp

HEADERS += \
    mainwindow.h \
    singleton.h \
    singletonfactory.h \
    exception.h \
    abstractprojectfactory.h \
    projectmanager.h \
    projectpointer.h \
    project.h

#AbstractBlockFactory
#AbstractBlock
#Gui::AbstractView
#Gui::AbstractEdit

#BlockModel (QAbstractItemModel)
#Gui::BlockView (QTreeView)


#....
#CppQt::Namespace::Block
#CppQt::Namespace::View
#CppQt::Namespace::Edit
