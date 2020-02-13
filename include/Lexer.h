#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include <istream> 
#include <string>

struct Lexeme
{
    enum {
        LEX_KW_SYM, // keyword or a literal symbol
        LEX_ID,
        LEX_NUMBER,
        LEX_STRING,
        LEX_EOF
    } LType;
    std::string LVal;
};

std::string LTypeToStr(int ltype);

std::ostream& operator<<(std::ostream& os, Lexeme& l);

class Lexer
{
public:
    Lexer(std::istream& inp) : inp(inp), line(1) {}
    Lexeme* next();
    int getLine();

private:
    std::istream& inp;
    int line;
};

#endif // LEXER_H_