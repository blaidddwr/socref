QT += core gui widgets

CONFIG += c++11

TARGET = socref
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    abstractprojectfactory.cpp \
    abstractproject.cpp \
    projectmanager.cpp \
    projectpointer.cpp

HEADERS += \
    mainwindow.h \
    singleton.h \
    singletonfactory.h \
    exception.h \
    abstractprojectfactory.h \
    abstractproject.h \
    projectmanager.h \
    projectpointer.h

#AbstractProject
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
