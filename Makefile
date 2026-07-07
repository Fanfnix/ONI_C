CC=gcc
CFLAGS=-Wall -Iinclude/ -Idata/cJson/
LDFLAGS=-lm -lncurses

BINDIR=bin
SRCDIR=src
OBJDIR=obj

LIBDIR=data/cJson

APPNAME=oni

SRC=$(shell find $(SRCDIR) -name '*.c')
OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBSRC=$(LIBDIR)/cJSON.c
LIBOBJ=$(LIBSRC:$(LIBDIR)/%.c=$(OBJDIR)/cjson/%.o)

.PHONY: all clean mrpropre

all: $(BINDIR)/$(APPNAME)

$(BINDIR)/$(APPNAME): $(OBJ) $(LIBOBJ) | $(BINDIR)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $

$(OBJDIR)/cjson/%.o: $(LIBDIR)/%.c | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $

$(BINDIR) $(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR)

mrpropre: clean
	rm -rf $(BINDIR) $(OBJDIR)