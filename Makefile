SUBDIRS := data/socref \
           data/socref_cpp \
           data/socref_cppqt \
           data/socref_glsl \
           data/socref_python

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
