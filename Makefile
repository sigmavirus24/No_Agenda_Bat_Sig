PWD := $(shell pwd)
MAKE := make -C
MAKECLEAN := make clean
DIRS := src/
Q  := @
CD := cd

default: all

all: 
	for i in $(DIRS) ; do \
		$(MAKE) $(PWD)/$$i ; \
	done

.SILENT:
clean:
	for i in $(DIRS) ; do \
		$(CD) $(PWD)/$$i && $(MAKECLEAN) ; \
	done

test:
	for i in $(DIRS) ; do \
		$(CD) $$i && make test ; \
	done
