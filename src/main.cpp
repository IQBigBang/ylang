#include "antlr4-runtime.h"
#include "YlangLexer.h"
#include "YlangParser.h"
#include "Visitor.h"

int main(int argc, const char* argv[]) {
    std::ifstream stream;
    stream.open("test/src.yy");
    
    antlr4::ANTLRInputStream input(stream);
    YlangLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    YlangParser parser(&tokens);
    YlangParser::CodeContext* tree = parser.code();
    YlangVisitor visitor;
    visitor.prepareEmit();
    visitor.visitCode(tree);
    visitor.print();
    visitor.Emit("test/bin.o");
    // link object file
    
    return 0;
}