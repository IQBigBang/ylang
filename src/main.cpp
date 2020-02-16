#include "Lexer.h"
#include "Parser.h"
#include "Visitor.h"
#include "args.hpp"
#include "Errors.h"
#include <csignal>
#include <fstream>

void run(int argc, char** argv) {
    args::ArgumentParser argParser("Ylang compiler");

    args::Positional<std::string> source(argParser, "source", "Source file path", args::Options::Required);
    args::ValueFlag<std::string> output(argParser, "output", "Output file path", {'o'});

    args::Group outputFormatsGroup(argParser, "Output formats", args::Group::Validators::AtMostOne);
    args::Flag outputFormatIR(outputFormatsGroup, "output IR", "Emit LLVM Immediate Representation", {"ir"});
    args::Flag outputFormatObj(outputFormatsGroup, "output object", "Emit native object file", {"obj"});
    args::Flag outputFormatRun(outputFormatsGroup, "run immediately", "Run code immediately after compilation and linking (keeps the compiled executable)", {'r'});

    args::Group optimizationsGroup(argParser, "Optimization settings", args::Group::Validators::AtMostOne);
    args::Flag optimizationsZero(optimizationsGroup, "no optimizations", "Disable all code optimizations (including LTO)", {"O0"});
    args::Flag optimizationsMin(optimizationsGroup, "minimal optimizations", "Enable only basic optimization passes (default)", {"O1"});
    args::Flag optimizationsFull(optimizationsGroup, "full optimizations", "Enable full optimizations (enables inliner)", {"O2"});
    args::Flag optimizationsSize(optimizationsGroup, "optimize for size", "Enables optimizations to reduce code size rather than faster execution", {"Os"});
    args::Flag noInliner(argParser, "no inliner", "Disable the inliner", {"no-inline"});
    args::Flag noLTO(argParser, "no lto", "Disables Link-Time-Optimization", {"no-lto"});

    args::Flag noGC(argParser, "no gc", "Prevents the garbage collector from being linked in. Correct linking requires you to supply a custom `alloc` function", {"no-gc"});
    args::Flag noStd(argParser, "no stdlib", "Prevents the standard library from being linked in. Garbage collector is still used (unless specified otherwise)", {"no-std"});

    args::HelpFlag help(argParser, "show the help", "Print the help message", {'h', "help"});

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
    err::fileName = args::get(source); // set the current filename
    
    Lexer lexer(stream);
    Parser parser(lexer);
    std::vector<ParseNode*> n = parser.parse();
    
    Visitor visitor;

    std::string outfile;
    if (output) 
        outfile = args::get(output); 
    else if (outputFormatIR)
        outfile = "bin.ll";
    else if (outputFormatObj)
        outfile = "bin.o";
    else outfile = "bin";
    
    for (auto& pn : n)
        visitor.visit(pn);

    if (!optimizationsZero) { // if opt = zero, don't even call the optimize function
        if (optimizationsFull) {
            visitor.optimize(2, 1, !noInliner);
        } else if (optimizationsSize) {
            visitor.optimize(1, 2, !noInliner);
        } else { // default
            visitor.optimize(1, 0, false);
        }
    }

    if (outputFormatIR)
        visitor.print(outfile);
    else if (outputFormatObj)
        visitor.Emit(outfile);
    else {
        visitor.Emit("tmp.o");
        std::string cmd = "clang "; // use clang
        if (!optimizationsZero) {
            cmd.append("-O2 ");
            if (!noLTO) {
                cmd.append("-flto=thin ");
            }
        }
        cmd.append("tmp.o "); // emitted temporary object file
        if (!noStd)
            cmd.append(STDLIBDIR "std.o "); // link standard library
        if (!noGC)
            cmd.append(STDLIBDIR "gc.o "); // link garbage collector
        cmd.append("-o " + outfile); // output file
        system(cmd.c_str());
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