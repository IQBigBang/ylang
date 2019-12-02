CXX = clang++-9
CPPFLAGS = -g `llvm-config --cxxflags --ldflags --libs --libfiles --system-libs` -fexceptions --std=c++14

OBJECTS = Lexer.o Parser.o Visitor.o main.o
OBJDIR = obj/

SRCDIR = src/
INCLUDEDIR = include/

OUTFILE = bin/ylang

INCLUDES =
LIBS = 

### 

PREFIXED_OBJECTS = $(addprefix $(OBJDIR),$(OBJECTS))

all: $(OUTFILE)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) -c $(CPPFLAGS) -Iinclude/ -o $@ $<

$(OUTFILE): $(PREFIXED_OBJECTS)
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LIBS)

std: stdlib/Std.c
	clang-9 -O3 -Weverything -Wno-missing-prototypes -c stdlib/Std.c -o stdlib/Std.o

clean:
	rm -rf $(OBJDIR)*.*

