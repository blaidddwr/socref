TEMPLATE = subdirs

SUBDIRS = \
    app \
    corelib \
    cpplib \
    cppqtlib

app.depends = cpplib cppqtlib corelib
