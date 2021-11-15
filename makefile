IDIR =include
CC=gcc
CFLAGS=-g -I$(IDIR) `sdl2-config --cflags --libs` 

ODIR=obj
LDIR =lib
SRCDIR=src

LIBS=-lm

_DEPS = text_manager.h SDL_helpers.h bit_manipulation.h MIPS_parser.h MIPS_memory.h MIPS_instructions.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = mips.o text_manager.o SDL_helpers.o bit_manipulation.o MIPS_parser.o MIPS_memory.o MIPS_instructions.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

#$(info $$OBJ/%.o is [${OBJ}])

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

mips: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ mips
	rmdir $(ODIR)
