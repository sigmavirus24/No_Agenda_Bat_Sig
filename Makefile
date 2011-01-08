PWD := $(shell pwd)
MAKE := make -C
DIRS := src/irc/ src/bat_signal/
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
		$(MAKE) $(PWD)/$$i $@ ; \
	done

test:
	for i in $(DIRS) ; do \
		$(MAKE) $(PWD)/$$i $@ ; \
	done
