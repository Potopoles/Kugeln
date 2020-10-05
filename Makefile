IDIR =include
CXX         =g++
CXX_FLAGS   =-I$(IDIR) -ggdb -fopenmp

ODIR=obj
SRCDIR=src
LDIR =lib

LIBS=-lm 

_DEPS = \
constants.h namelist.h \
StopWatch.h utilities.h debug.h\
integrator.h \
spatial_tendencies.h \
state.h grid.o\
initial_condition.h \
io.h \
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o \
StopWatch.o utilities.o debug.o\
integrator.o \
spatial_tendencies.o \
state.o grid.o\
initial_condition.o \
io.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


main: $(OBJ)
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(LIBS)

$(OBJ): $(ODIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXX_FLAGS) -I lib


run:
	./main 100
	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
