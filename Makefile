CXX := g++.exe
CXXFLAGS := -std=c++20 -Wall -pedantic -g
LDFLAGS := -static-libasan
LIBS := -lexpression_parser
LIBDIR := -LC:/Users/chean/CLionProjects/BIGHW/x86_64-linux-gnu
SRCDIR := C:/Users/chean/CLionProjects/BIGHW
BINDIR := $(SRCDIR)

# Source and object files
SRC := $(SRCDIR)/test.cpp
OBJ := $(SRC:.cpp=.o)
EXE := $(BINDIR)/excel.exe

# Default target
all: $(EXE)

# Link the executable
$(EXE): $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) -o $@ $(LIBDIR) $(LIBS)

# Compile the source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $
	<CXX> -o $@

# Clean target for cleaning up the built files
clean:
	del $(OBJ) $(EXE)

.PHONY: all clean