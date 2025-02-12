TEMPLATE = app
QT += core gui widgets testlib
CONFIG += c++17 testcase no_testcase_installs
INCLUDEPATH += ../../corelib
LIBS += -L$$OUT_PWD/../../libs -lcore -lhunspell
PRE_TARGETDEPS += $$OUT_PWD/../../libs/libcore.a
SOURCES += test.cpp
HEADERS += \
    ../TestBlock.h \
    ../TestLanguage.h
