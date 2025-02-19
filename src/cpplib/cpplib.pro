TEMPLATE = lib
QT += core gui widgets
CONFIG += c++17 staticlib
INCLUDEPATH += ../corelib
SOURCES += \
    Cpp.cpp \
    CppBlockBase.cpp \
    CppBlockClass.cpp \
    CppBlockEnumeration.cpp \
    CppBlockEnumerationValue.cpp \
    CppBlockException.cpp \
    CppBlockFunction.cpp \
    CppBlockNamespace.cpp \
    CppBlockProperty.cpp \
    CppBlockUnion.cpp \
    CppBlockVariable.cpp \
    CppBlockWidgetBaseEdit.cpp \
    CppBlockWidgetClassEdit.cpp \
    CppBlockWidgetEnumerationEdit.cpp \
    CppBlockWidgetEnumerationValueEdit.cpp \
    CppBlockWidgetExceptionEdit.cpp \
    CppBlockWidgetFunctionEdit.cpp \
    CppBlockWidgetNamespaceEdit.cpp \
    CppBlockWidgetPropertyEdit.cpp \
    CppBlockWidgetUnionEdit.cpp \
    CppBlockWidgetVariableEdit.cpp \
    CppLanguage.cpp \
    CppParseHeadParser.cpp \
    CppRouter.cpp
HEADERS += \
    Cpp.h \
    CppBlock.h \
    CppBlockBase.h \
    CppBlockClass.h \
    CppBlockEnumeration.h \
    CppBlockEnumerationValue.h \
    CppBlockException.h \
    CppBlockFunction.h \
    CppBlockNamespace.h \
    CppBlockProperty.h \
    CppBlockUnion.h \
    CppBlockVariable.h \
    CppBlockWidget.h \
    CppBlockWidgetBaseEdit.h \
    CppBlockWidgetClassEdit.h \
    CppBlockWidgetEnumerationEdit.h \
    CppBlockWidgetEnumerationValueEdit.h \
    CppBlockWidgetExceptionEdit.h \
    CppBlockWidgetFunctionEdit.h \
    CppBlockWidgetNamespaceEdit.h \
    CppBlockWidgetPropertyEdit.h \
    CppBlockWidgetUnionEdit.h \
    CppBlockWidgetVariableEdit.h \
    CppLanguage.h \
    CppParse.h \
    CppParseHeadParser.h \
    CppRouter.h
RESOURCES += cpp.qrc
DESTDIR = $$OUT_PWD/../libs
TARGET = cpp
