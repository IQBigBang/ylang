CXX = clang++-9
CPPFLAGS = -g `llvm-config-9 --cxxflags --ldflags --libs --libfiles --system-libs` -fexceptions --std=c++14

OBJECTS = Lexer.o Parser.o Visitor.o main.o Errors.o
OBJDIR = obj/

SRCDIR = src/
INCLUDEDIR = include/

OUTFILE = bin/ylang

INCLUDES =
LIBS = 

### 

PREFIXED_OBJECTS = $(addprefix $(OBJDIR),$(OBJECTS))

CPPFLAGS := $(CPPFLAGS) -DSTDLIBDIR="\"$(CURDIR)/stdlib/\""

all: prepare $(OUTFILE)

prepare:
	mkdir -p obj bin 

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) -c $(CPPFLAGS) -Iinclude/ -Wno-unused-command-line-argument -o $@ $<

$(OUTFILE): $(PREFIXED_OBJECTS) stdlib/std.o stdlib/gc.o
	$(CXX) $(CPPFLAGS) $(LIBS) $(PREFIXED_OBJECTS) -o $@

stdlib/std.o: stdlib/Std.c
	clang-9 -O3 -Weverything -Wno-missing-prototypes -flto=thin -c stdlib/Std.c -o stdlib/std.o

stdlib/gc.o: stdlib/gc/tgc.c
	clang-9 -O3 -Weverything -Wno-missing-prototypes -flto=thin -c stdlib/gc/tgc.c -o stdlib/gc.o

clean:
	rm -rf $(OBJDIR)*.* stdlib/*.o

