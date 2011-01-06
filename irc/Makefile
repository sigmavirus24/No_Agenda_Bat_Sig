VERSION := 0.1.development
RM := rm -f
CC := gcc
CFLAGS := -ggdb3 -Werror -Wall -DVERSION=\"$(VERSION)\" `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`
Q := @
# PREFIX := /usr/local

TARGETS := nabot
SRC := src/main.c src/utils.c
OBJ := $(SRC:%c=%o)

default: all

all: $(TARGETS)

nabot: $(OBJ)
	@echo "  [BUILD] $@"
#$(Q)mv main.c .main.c
#$(Q)sed "s/VERSION/${VERSION}/g" < .main.c > main.c
#$(Q)$(RM) .main.c
	$(Q)$(CC) $(OBJ) $(CFLAGS) -o $@ -lcurl
	$(Q)mv $@ ../

.SILENT:
clean:
	for i in $(TARGETS) ; do \
		$(RM) *.o $$i ../$$i ; \
	done

%.o: %.c
	@echo "  [CC] $@"
	$(Q)$(COMPILE.c) $(OUTPUT_OPTION) $<
