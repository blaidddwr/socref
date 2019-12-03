SUBDIRS := socref socref_python

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
