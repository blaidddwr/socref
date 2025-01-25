TEMPLATE = subdirs

SUBDIRS = \
    app \
    lib

app.depends = lib
