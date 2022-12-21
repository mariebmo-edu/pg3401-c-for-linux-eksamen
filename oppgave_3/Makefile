INCLDIR = ./include
CC = gcc
CFLAGS = -O2 -g
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

_DEPS =  dateTimeHandling.h menu.h reservation.h oppgave3.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS =  dateTimeHandling.o menu.o reservation.o oppgave3.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

oppgave3: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~
