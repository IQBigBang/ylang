#include "antlr4-runtime.h"
#include "YlangLexer.h"
#include "YlangParser.h"
#include "Visitor.h"
#include "args.hxx"

int main(int argc, char** argv) {
    args::ArgumentParser argParser("YLang compiler");
    args::Positional<std::string> source(argParser, "source", "Source file path", args::Options::Required);
    args::Group outputFormatsGroup(argParser, "Output file type formats", args::Group::Validators::AtMostOne);
    args::Flag outputFormatIR(outputFormatsGroup, "output IR", "Emit LLVM Immediate Representation", {"ir"});
    args::Flag outputFormatObj(outputFormatsGroup, "output object", "Emit native object file", {"obj"});
    args::Flag outputFormatRun(outputFormatsGroup, "run immediately", "Run code immediately after compilation and linking", {'r'});
    args::ValueFlag<std::string> output(argParser, "output", "Output file path", {'o'});

    try
    {
        argParser.ParseCLI(argc, argv);
    }
    catch (args::Help)
    {
        std::cout << argParser;
        return 0;
    }
    catch (args::ParseError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << argParser;
        return 1;
    }
    catch (args::ValidationError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << argParser;
        return 1;
    }

    std::ifstream stream;
    stream.open(args::get(source));
    
    antlr4::ANTLRInputStream input(stream);
    YlangLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    YlangParser parser(&tokens);
    YlangParser::CodeContext* tree = parser.code();
    YlangVisitor visitor;

    std::string outfile;
    if (outputFormatRun) outfile = "test/bin";
    else if (output) outfile = args::get(output); 
    else {
        if (outputFormatIR)
            outfile = "test/bin.ll";
        else if (outputFormatObj)
            outfile = "test/bin.o";
        else outfile = "test/bin";
    }

    if (!outputFormatIR) // if output is object or executable, obj file is generated
        visitor.prepareEmit();
    
    visitor.visitCode(tree);

    if (outputFormatIR)
        visitor.print(outfile);
    else if (outputFormatObj)
        visitor.Emit(outfile);
    else {
        visitor.Emit("tmp.o");
        system(("g++ tmp.o stdlib/Std.o -o " + outfile).c_str());
        system("rm tmp.o");
        if (outputFormatRun)
        {
            system("./test/bin");
            system("rm test/bin");
        }
    }
    
    return 0;
}