RM := rm -f
CC := gcc
CFLAGS := -ggdb3 -Werror -Wall -DDEBUG `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`
Q := @

TARGETS := main
SRC := main.c curl_functions.c parse.c xmalloc.c finds.c free_t_tweet.c gtk_fns.c
OBJ := $(SRC:%c=%o)

default: all

all: $(TARGETS)

main: $(OBJ)
	@echo "  [BUILD] $@"
	$(Q)$(CC) $(OBJ) $(CFLAGS) -o $@ -lcurl
	mv $@ ../
# -lcurl necessary to compile, don't ask me why

clean:
	$(RM) *.o $(TARGETS) ../$(TARGETS)

%.o: %.c
	@echo "  [CC] $@"
	$(Q)$(COMPILE.c) $(OUTPUT_OPTION) $<
