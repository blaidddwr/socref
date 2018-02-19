QT += core gui widgets xml

CONFIG += c++11

TARGET = socref
TEMPLATE = app

INCLUDEPATH += ../../socerr/src
LIBS += -L../../socerr/build -lsocerr -laspell

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
    cppqt_edit_variable.cpp \
    cppqt_view_base.cpp \
    cppqt_view_variable.cpp \
    cppqt_signal.cpp \
    cppqt_function.cpp \
    cppqt_view_function.cpp \
    cppqt_template.cpp \
    cppqt_view_template.cpp \
    cppqt_edit_function.cpp \
    cppqt_edit_template.cpp \
    cppqt_class.cpp \
    cppqt_access.cpp \
    cppqt_view_class.cpp \
    cppqt_common.cpp \
    cppqt_view_common.cpp \
    cppqt_view_access.cpp \
    cppqt_gui_listdialog.cpp \
    gui_textedit.cpp \
    gui_textedit_highlighter.cpp \
    gui_textedit_dialog.cpp \
    gui_textdialog.cpp \
    cppqt_gui_listdialog_model.cpp \
    cppqt_edit_class.cpp \
    cppqt_edit_access.cpp \
    cppqt_operator.cpp \
    cppqt_view_operator.cpp \
    cppqt_edit_operator.cpp \
    cppqt_constructor.cpp \
    cppqt_view_constructor.cpp \
    cppqt_edit_constructor.cpp \
    cppqt_destructor.cpp \
    cppqt_edit_destructor.cpp \
    cppqt_view_destructor.cpp \
    cppqt_slot.cpp \
    cppqt_view_slot.cpp \
    cppqt_edit_slot.cpp \
    cppqt_view_signal.cpp \
    cppqt_edit_signal.cpp \
    cppqt_enumeration.cpp \
    cppqt_view_enumeration.cpp \
    cppqt_edit_enumeration.cpp \
    cppqt_enumvalue.cpp \
    cppqt_view_enumvalue.cpp \
    cppqt_edit_enumvalue.cpp \
    cppqt_parent.cpp \
    cppqt_view_parent.cpp \
    cppqt_edit_parent.cpp \
    abstractparser.cpp \
    cppqt_parserfactory.cpp \
    scanthread.cpp \
    common.cpp \
    gui_scandialog.cpp \
    cppqt_parse_global.cpp \
    cppqt_parse_function.cpp \
    cppqt_parse_base.cpp \
    cppqt_parse_header.cpp \
    cppqt_parse_variable.cpp \
    cppqt_parse_enumeration.cpp \
    cppqt_parse_common.cpp

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
    cppqt_edit_variable.h \
    cppqt_view_base.h \
    cppqt_view_variable.h \
    cppqt_signal.h \
    cppqt_function.h \
    cppqt_view_function.h \
    cppqt_template.h \
    cppqt_view_template.h \
    cppqt_edit_function.h \
    cppqt_edit_template.h \
    cppqt_class.h \
    cppqt_access.h \
    cppqt_view_class.h \
    cppqt_common.h \
    cppqt_view_common.h \
    cppqt_view_access.h \
    cppqt_gui_listdialog.h \
    gui_textedit.h \
    gui_textedit_highlighter.h \
    gui_textedit_dialog.h \
    gui_textdialog.h \
    cppqt_gui_listdialog_model.h \
    cppqt_edit_class.h \
    cppqt_edit_access.h \
    cppqt_operator.h \
    cppqt_view_operator.h \
    cppqt_edit_operator.h \
    cppqt_constructor.h \
    cppqt_view_constructor.h \
    cppqt_edit_constructor.h \
    cppqt_destructor.h \
    cppqt_edit_destructor.h \
    cppqt_view_destructor.h \
    cppqt_slot.h \
    cppqt_view_slot.h \
    cppqt_edit_slot.h \
    cppqt_view_signal.h \
    cppqt_edit_signal.h \
    cppqt_enumeration.h \
    cppqt_view_enumeration.h \
    cppqt_edit_enumeration.h \
    cppqt_enumvalue.h \
    cppqt_view_enumvalue.h \
    cppqt_edit_enumvalue.h \
    cppqt_parent.h \
    cppqt_view_parent.h \
    cppqt_edit_parent.h \
    abstractparser.h \
    abstractparserfactory.h \
    cppqt_parserfactory.h \
    scanthread.h \
    common.h \
    gui_scandialog.h \
    cppqt_parse.h \
    cppqt_parse_global.h \
    cppqt_parse_function.h \
    cppqt_parse_base.h \
    cppqt_parse_header.h \
    cppqt_parse_variable.h \
    cppqt_parse_enumeration.h \
    cppqt_parse_common.h

RESOURCES += \
    resources.qrc
