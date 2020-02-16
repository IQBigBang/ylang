#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include <istream> 
#include <string>
#include <vector>

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

    ~Lexeme() {} // don't delete LVal
};

std::string LTypeToStr(int ltype);

std::ostream& operator<<(std::ostream& os, Lexeme& l);

class Lexer
{
public:
    Lexer(std::istream& inp) : inp(inp), line(1) {}
    Lexeme* next();
    int getLine();

    const static std::vector<std::string> keywords; // array of reserved keywords

private:
    std::istream& inp;
    int line;
};

#endif // LEXER_H_