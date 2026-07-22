CC=gcc
CFLAGS=-Wall -Wextra -g -Iinclude/ -fopenmp -MMD -MP
SDLFLAGS=$(shell pkg-config --cflags --libs sdl2)
LDFLAGS=-lm -lncurses -fopenmp $(SDLFLAGS)

SRCDIR=src
OBJDIR=obj

APPNAME=oni

SRC=$(shell find $(SRCDIR) -name '*.c')
OBJ=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
DEP=$(OBJ:.o=.d)

.PHONY: all clean mrpropre

all: $(APPNAME)

$(APPNAME): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $@

-include $(DEP)

clean:
	rm -rf $(OBJDIR)

mrpropre: clean
	rm -f $(APPNAME)