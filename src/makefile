INC_DIR =../include
CC = g++ -std=c++11
CFLAGS = -g -I $(INC_DIR)

ODIR = obj
LDIR =../lib

LIBS = -lm

_DEPS = io_manager.hpp definitions.hpp assembler.hpp pre_processor.hpp
DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))

_OBJ = main.o io_manager.o assembler.o pre_processor.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

montador: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 