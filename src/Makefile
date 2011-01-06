VERSION := 0.3.development
RM := rm -f
CC := gcc
CFLAGS := -ggdb3 -Werror -Wall `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0` -DVERSION=\"$(VERSION)\" 
Q := @
# PREFIX := /usr/local

TARGETS := nabatsignal test
SRC := main.c socket_functions.c curl_functions.c parse.c xmalloc.c finds.c free_t_tweet.c gtk_fns.c bat_sig_rc.c
OBJ := $(SRC:%c=%o)

default: all

all: $(TARGETS)

nabatsignal: $(OBJ)
	@echo "  [BUILD] $@"
#$(Q)mv main.c .main.c
#$(Q)sed "s/VERSION/${VERSION}/g" < .main.c > main.c
#$(Q)$(RM) .main.c
	$(Q)$(CC) $(OBJ) $(CFLAGS) -o $@ -lcurl
	$(Q)mv $@ ../

test: $(OBJ)
	@echo "  [BUILD] $@"
	$(Q)$(CC) $(SRC) -DSIGMANATEST $(CFLAGS) -o $@ -lcurl
	$(Q)mv $@ ../

.SILENT:
clean:
	for i in $(TARGETS) ; do \
		$(RM) *.o $$i ../$$i ; \
	done

%.o: %.c
	@echo "  [CC] $@"
	$(Q)$(COMPILE.c) $(OUTPUT_OPTION) $<
