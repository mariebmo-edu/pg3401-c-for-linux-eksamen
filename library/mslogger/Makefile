INCLDIR = ./include
CC = gcc
CFLAGS = -O2 -ansi
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

_DEPS = mslogger.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = mslogger.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

libmslogger.a: $(OBJS)
	ar rcs libmslogger.a -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~
