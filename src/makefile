IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR=../lib

LIBS=-lm

_DEPS = tree-greedy.h tree-generator.h tree-helper.h graph.h stack.h int_list.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = tree-augmentor.o tree-generator.o tree-helper.o tree-greedy.o data-structures/graph.o data-structures/stack.o data-structures/int_list.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

tree-augmentor: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
