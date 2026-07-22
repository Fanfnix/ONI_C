CC=gcc
CFLAGS=-Wall -Wextra -g -Iinclude/ -fopenmp
SDLFLAGS=$(shell pkg-config --cflags --libs sdl2)
LDFLAGS=-lm -lncurses -fopenmp $(SDLFLAGS)

SRCDIR=src
OBJDIR=obj

APPNAME=oni

SRC=$(shell find $(SRCDIR) -name '*.c')
OBJ=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(filter $(SRCDIR)/%.c,$(SRC)))

.PHONY: all clean mrpropre

all: $(APPNAME)

$(APPNAME): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR)

mrpropre: clean
	rm -f $(APPNAME)