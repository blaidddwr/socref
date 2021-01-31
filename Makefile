SUBDIRS := data/socref data/socref_python

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
