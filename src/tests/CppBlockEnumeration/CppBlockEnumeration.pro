TEMPLATE = app
QT += core gui widgets testlib
CONFIG += c++17 testcase no_testcase_installs
INCLUDEPATH += ../../corelib ../../cpplib
LIBS += -L$$OUT_PWD/../../libs -lcpp -lcore -lhunspell
PRE_TARGETDEPS += $$OUT_PWD/../../libs/libcore.a $$OUT_PWD/../../libs/libcpp.a
SOURCES += test.cpp
