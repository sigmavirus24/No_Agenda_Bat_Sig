RM := rm -f
CC := gcc
CFLAGS := -ggdb3 -Werror -Wall -DDEBUG
Q := @

TARGETS := main
SRC := main.c curl_functions.c
OBJ := $(SRC:%c=%o)

default: all

all: $(TARGETS)

main: $(OBJ)
	@echo "  [BUILD] $@"
	$(Q)$(CC) $(OBJ) $(CFLAGS) -o $@ -lcurl
# -lcurl necessary to compile, don't ask me why

clean:
	$(RM) *.o $(TARGETS)

%.o: %.c
	@echo "  [CC] $@"
	$(Q)$(COMPILE.c) $(OUTPUT_OPTION) $<
