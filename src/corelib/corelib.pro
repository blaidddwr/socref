TEMPLATE = lib
QT += core gui widgets
CONFIG += c++17 staticlib
SOURCES += \
    AbstractBlock.cpp \
    AbstractBlockWidget.cpp \
    AbstractLanguage.cpp \
    AbstractParser.cpp \
    AbstractRouter.cpp \
    CommandAbstractProject.cpp \
    CommandProjectInsert.cpp \
    CommandProjectMove.cpp \
    CommandProjectRemove.cpp \
    CommandProjectSet.cpp \
    ControllerCode.cpp \
    ControllerSpellingHighlight.cpp \
    Exception.cpp \
    FactoryLanguage.cpp \
    ModelCode.cpp \
    ModelMetaBlock.cpp \
    ModelMetaLanguage.cpp \
    ModelProject.cpp \
    SettingsSpelling.cpp \
    StreamBlockDir.cpp \
    StreamBlockXml.cpp \
    StreamProjectDir.cpp \
    StreamProjectXml.cpp \
    WidgetDialogAbout.cpp \
    WidgetDialogCode.cpp \
    WidgetDialogOrphanFiles.cpp \
    WidgetDialogProject.cpp \
    WidgetDialogSpellingSettings.cpp \
    WidgetProject.cpp \
    WidgetStringList.cpp \
    WidgetTreeView.cpp \
    WidgetWindowMain.cpp
HEADERS += \
    AbstractBlock.h \
    AbstractBlockWidget.h \
    AbstractLanguage.h \
    AbstractParser.h \
    AbstractRouter.h \
    Command.h \
    CommandAbstractProject.h \
    CommandProject.h \
    CommandProjectInsert.h \
    CommandProjectMove.h \
    CommandProjectRemove.h \
    CommandProjectSet.h \
    Controller.h \
    ControllerCode.h \
    ControllerSpellingHighlight.h \
    Exception.h \
    Factory.h \
    FactoryLanguage.h \
    Global.h \
    Model.h \
    ModelCode.h \
    ModelMeta.h \
    ModelMetaBlock.h \
    ModelMetaLanguage.h \
    ModelProject.h \
    Settings.h \
    SettingsSpelling.h \
    Shape.h \
    Stream.h \
    StreamBlockDir.h \
    StreamBlockXml.h \
    StreamProjectDir.h \
    StreamProjectXml.h \
    Widget.h \
    WidgetDialog.h \
    WidgetDialogAbout.h \
    WidgetDialogCode.h \
    WidgetDialogOrphanFiles.h \
    WidgetDialogProject.h \
    WidgetDialogSpellingSettings.h \
    WidgetProject.h \
    WidgetStringList.h \
    WidgetTreeView.h \
    WidgetWindow.h \
    WidgetWindowMain.h
RESOURCES += core.qrc
DESTDIR = $$OUT_PWD/../libs
TARGET = core
