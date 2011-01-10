VERSION := 0.2
RM := rm -f
CC := gcc
CFLAGS := -Os -Werror -Wall -DVERSION=\"$(VERSION)\" -D_GNU_SOURCE #-DDEBUG
Q := @
# PREFIX := /usr/local

TARGETS := nabot
SRC := main.c utils.c
OBJ := $(SRC:%c=%o)

default: all

all: $(TARGETS)

nabot: $(OBJ)
	@echo "  [BUILD] $@"
	$(Q)$(CC) $(OBJ) $(CFLAGS) -o $@
	$(Q)mv $@ ../../

.SILENT:
clean:
	for i in $(TARGETS) ; do \
		$(RM) $(OBJ) $$i ../../$$i ; \
	done

%.o: %.c
	@echo "  [CC] $@"
	$(Q)$(COMPILE.c) $(OUTPUT_OPTION) $<
