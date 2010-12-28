RM := rm -f
CC := gcc
CFLAGS := -ggdb3 -Werror -Wall -DDEBUG `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`
Q := @
VERSION := 0.3
# PREFIX := /usr/local

TARGETS := nabatsignal
SRC := main.c socket_functions.c curl_functions.c parse.c xmalloc.c finds.c free_t_tweet.c gtk_fns.c bat_sig_rc.c
OBJ := $(SRC:%c=%o)

default: all

all: $(TARGETS)

nabatsignal: $(OBJ)
	@echo "  [BUILD] $@"
#$(Q)mv main.c include/
#$(Q)sed "s/VERSION/${VERSION}/g" < include/main.c > main.c
#$(Q)rm include/main.c
	$(Q)$(CC) $(OBJ) $(CFLAGS) -o $@ -lcurl
	mv $@ ../
# -lcurl necessary to compile, don't ask me why

test: $(OBJ)
	@echo "  [BUILD] $@"
	$(Q)$(CC) $(SRC) -DSIGMANATEST $(CFLAGS) -o $@ -lcurl
	mv $@ ../

clean:
	$(RM) *.o $(TARGETS) ../$(TARGETS)

%.o: %.c
	@echo "  [CC] $@"
	$(Q)$(COMPILE.c) $(OUTPUT_OPTION) $<
