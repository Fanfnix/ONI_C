CC=gcc
CFLAGS=-Wall -Wextra -g -Iinclude/ -Idata/cJson
LDFLAGS=-lm -lncurses

SRCDIR=src
CJSONDIR=data/cJson
OBJDIR=obj

APPNAME=oni

SRC=$(shell find $(SRCDIR) -name '*.c') $(CJSONDIR)/cJSON.c
OBJ=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(filter $(SRCDIR)/%.c,$(SRC))) \
    $(patsubst $(CJSONDIR)/%.c,$(OBJDIR)/cjson/%.o,$(filter $(CJSONDIR)/%.c,$(SRC)))

.PHONY: all clean mrpropre

all: $(APPNAME)

$(APPNAME): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/cjson/%.o: $(CJSONDIR)/%.c | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR)

mrpropre: clean
	rm -f $(APPNAME)