TEMPLATE = app
QT += core gui widgets testlib
CONFIG += c++17 testcase no_testcase_installs
INCLUDEPATH += ../../corelib ../../cpplib ../../cppqtlib
LIBS += -L$$OUT_PWD/../../libs -lcppqt -lcpp -lcore -lhunspell
PRE_TARGETDEPS += \
    $$OUT_PWD/../../libs/libcore.a \
    $$OUT_PWD/../../libs/libcpp.a \
    $$OUT_PWD/../../libs/libcppqt.a
SOURCES += test.cpp
