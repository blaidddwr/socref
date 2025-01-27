TEMPLATE = app
QT += core gui widgets testlib
CONFIG += c++17 testcase no_testcase_installs
INCLUDEPATH += ../../lib
LIBS += -lhunspell -L$$OUT_PWD/../../lib -lsocref
PRE_TARGETDEPS += $$OUT_PWD/../../lib/libsocref.a
SOURCES += test.cpp
