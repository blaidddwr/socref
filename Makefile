SUBDIRS := socref socref_cpp socref_cppqt socref_python

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
