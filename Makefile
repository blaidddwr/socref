SUBDIRS := data/socref \
           data/socref_python \
           data/socref_glsl

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
