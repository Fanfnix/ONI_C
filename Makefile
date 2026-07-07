CC=gcc
CFLAGS=-Wall
LDFLAGS=-lm -lncurses

BINDIR=bin
SRCDIR=src
OBJDIR=obj

APPNAME=oni

SRC=$(shell find $(SRCDIR) -name *.c)
OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(APPNAME)

$(BINDIR)/$(APPNAME): $(OBJ) | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
        mkdir -p $@
        $(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR) $(OBJDIR):
    mkdir $@

clean:
	rm -rf $(OBJDIR)/*.o

mrpropre: clean
	rm -rf $(BINDIR) $(OBJDIR)
