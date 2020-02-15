#include "Errors.h"
#include "termcolor.hpp"
#include <iostream>

static int err_count = 0; // when the error count > 5, fail

std::string err::fileName = "";

void err_signal_handle(int signo)
{
    if (err_count > 0) std::exit(0); // exit peacefully
    else {
        err::throwNonfatal("Internal error", "Segmentation fault");
        std::abort();
    }
}

void err::throwNonfatal(std::string error, std::string description, int line)
{
    std::cerr << termcolor::bold << termcolor::red << "Error: ";
    std::cerr << termcolor::reset;
    std::cerr << termcolor::bold << error;
    if (line != -1)
        std::cerr << " in " << err::fileName << ":" << line;
    std::cerr << std::endl << termcolor::reset;
    if (description != "")
        std::cerr << description << std::endl;
    
    if (++err_count > 5) throw std::exception();
}

void err::throwFatal(std::string error, std::string description, int line)
{
    err_count = 6; // guarantee immediate fail
    err::throwNonfatal(error, description, line);
}