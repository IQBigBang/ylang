#include "Lexer.h"
#include "Parser.h"
#include "Visitor.h"
#include "args.hpp"
#include "Errors.h"
#include <csignal>
#include <fstream>

void run(int argc, char** argv) {
    args::ArgumentParser argParser("YLang compiler");
    args::Positional<std::string> source(argParser, "source", "Source file path", args::Options::Required);
    args::Group outputFormatsGroup(argParser, "Output file type formats", args::Group::Validators::AtMostOne);
    args::Flag outputFormatIR(outputFormatsGroup, "output IR", "Emit LLVM Immediate Representation", {"ir"});
    args::Flag outputFormatObj(outputFormatsGroup, "output object", "Emit native object file", {"obj"});
    args::Flag outputFormatRun(outputFormatsGroup, "run immediately", "Run code immediately after compilation and linking (keeps the compiled executable)", {'r'});
    args::HelpFlag help(argParser, "show the help", "Print the help message", {'h', "help"});
    args::Flag noOptimizations(argParser, "no optimizations", "Disable code optimizations", {"O0"});
    args::ValueFlag<std::string> output(argParser, "output", "Output file path", {'o'});

    try
    {
        argParser.ParseCLI(argc, argv);
    }
    catch (args::Help)
    {
        std::cout << argParser;
        return;
    }
    catch (args::ParseError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << argParser;
        return;
    }
    catch (args::ValidationError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << argParser;
        return;
    }

    std::ifstream stream;
    stream.open(args::get(source));
    if (stream.fail() || !stream.is_open()) {
        err::throwFatal("Could not open the source file");
    }
    
    Lexer lexer(stream);
    Parser parser(lexer);
    std::vector<ParseNode*> n = parser.parse();

    /*for (auto& pn : n)
        pn->print();*/
    
    Visitor visitor;

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
    
    for (auto& pn : n)
        visitor.visit(pn);
    
    visitor.optimize(!noOptimizations);

    if (outputFormatIR)
        visitor.print(outfile);
    else if (outputFormatObj)
        visitor.Emit(outfile);
    else {
        visitor.Emit("tmp.o");
        system(("clang -O2 -flto=thin tmp.o stdlib/std.o stdlib/gc.o -o " + outfile).c_str());
        system("rm tmp.o");
        if (outputFormatRun)
        {
            system(("./" + outfile).c_str());
        }
    }
}

int main(int argc, char** argv) {
    // when a non-fatal error appears
    // it can cause the program
    // to segfault later in execution
    // this silences segmentation fault if an error was previously thrown
    // comment out for debugging purposes
    signal(SIGSEGV, err_signal_handle);

    // this is used for proper crashing
    // when an error appears
    // the exception propagation guarantees RAII
    try {
        run(argc, argv);
        return 0;
    } catch (...) { 
        return 1;
    }
}