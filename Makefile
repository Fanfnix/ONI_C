CC=gcc
CFLAGS=-Wall -I./$(SRCDIR)
LDFLAGS=-lm -lncurses

BINDIR=bin
SRCDIR=src
OBJDIR=obj

APPNAME=oni

SRC=$(shell find $(SRCDIR) -name '*.c')
OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

.PHONY: all clean mrpropre

all: $(BINDIR)/$(APPNAME)

$(BINDIR)/$(APPNAME): $(OBJ) | $(BINDIR)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR) $(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR)/*.o

mrpropre: clean
	rm -rf $(BINDIR) $(OBJDIR)
