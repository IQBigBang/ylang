CXX = clang++-9
CPPFLAGS = -g `llvm-config --cxxflags --ldflags --libs --libfiles --system-libs` -fexceptions --std=c++14

OBJECTS = YlangLexer.o YlangParser.o Visitor.o main.o
OBJDIR = obj/

SRCDIR = src/
INCLUDEDIR = include/

OUTFILE = bin/ylang

INCLUDES = /usr/local/include/antlr4-runtime
LIBS = /usr/local/lib/libantlr4-runtime.a

ANTLR = antlr4

### 

PREFIXED_OBJECTS = $(addprefix $(OBJDIR),$(OBJECTS))

all: $(OUTFILE)

grammar: Ylang.g4
	$(ANTLR) -o $(SRCDIR) -no-listener -visitor -Dlanguage=Cpp $<
	sed -i 's|YlangVisitor.h|Visitor.h|g' $(SRCDIR)YlangParser.cpp
	rm $(SRCDIR)YlangBaseVisitor.* $(SRCDIR)YlangVisitor.* $(SRCDIR)Ylang.* $(SRCDIR)YlangLexer.interp $(SRCDIR)YlangLexer.tokens
	mv $(SRCDIR)YlangLexer.h $(INCLUDEDIR)YlangLexer.h
	mv $(SRCDIR)YlangParser.h $(INCLUDEDIR)YlangParser.h

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) -c $(CPPFLAGS) -I$(INCLUDES) -Iinclude/ -o $@ $<

$(OUTFILE): $(PREFIXED_OBJECTS)
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf $(OBJDIR)*.*

