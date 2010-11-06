RM := rm -f
CC := gcc
CFLAGS := -ggdb3 -Werror -Wall
Q := @

TARGETS := main
SRC := main.c
OBJ := $(SRC:%c=%o)

default: all

all: $(TARGETS)

main: $(OBJ)
	@echo "  [BUILD] $@"
	$(Q)$(CC) $(OBJ) $(CFLAGS) -o $@

clean:
	$(RM) *.o $(TARGETS)

%.o: %.c
	@echo "  [CC] $@"
	$(Q)$(COMPILE.c) $(OUTPUT_OPTION) $<
