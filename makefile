IDIR =include
CC=gcc
CFLAGS=-g -I$(IDIR) `sdl2-config --cflags --libs` 

ODIR=obj
LDIR =lib
SRCDIR=src

LIBS=-lm

_DEPS = SDL_helpers.h bit_manipulation.h MIPS_parser.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = mips.o SDL_helpers.o bit_manipulation.o MIPS_parser.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

#$(info $$OBJ/%.o is [${OBJ}])

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mips: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
