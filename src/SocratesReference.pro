TEMPLATE = subdirs

SUBDIRS = \
    app \
    corelib \
    cpplib \
    cppqtlib \
    tests

app.depends = cppqtlib cpplib corelib
tests.depends = cppqtlib cpplib corelib
