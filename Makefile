SUBDIRS := data/socref \
           data/socref_python \
           data/socref_glsl \
           data/socref_cpp

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
