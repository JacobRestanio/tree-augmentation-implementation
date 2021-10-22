IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR=../lib

LIBS=-lm

_DEPS = tree-greedy.h tree-check-connected.h tree-generator.h tree-helper.h fredrickson.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = tree-augmentor.o tree-generator.o tree-helper.o tree-greedy.o tree-check-connected.o fredrickson.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

tree-augmentor: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
