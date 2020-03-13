#include <ctype.h>
#include <fstream>
#include "Lexer.h"
#include "Errors.h"

std::ostream& operator<<(std::ostream& os, Lexeme& l)
{
    os << "{ " << l.LType;
    if (l.LType == Lexeme::LEX_EOF)
        os << " }";
    else
        os << " '" << l.LVal << "' }";
    return os;
}

std::string LTypeToStr(int ltype)
{
    switch (ltype) {
        case 0: return "Keyword/Symbol";
        case 1: return "Identifier";
        case 2: return "Number";
        case 3: return "String";
        default: return "EOF";
    }
}

int Lexer::getLine()
{
    return this->line;
}

const std::vector<std::string> Lexer::keywords = 
    {"fn", "type", "if", "else", "switch", "case", "let", "and", "or", "true", "false", "while", "include"};

Lexeme *Lexer::next()
{
    char c;
    inp.get(c);
    
    if (inp.eof() || c == std::istream::traits_type::eof() || c == 0)
        return new Lexeme{.LType = Lexeme::LEX_EOF};

    // skip whitespace and comments
    while (std::isspace(c) || c == '#')
    {
        if (std::isspace(c))
        {
            do {
                if (c == '\n') ++line; // line counting
            } while (std::isspace(c = inp.get()));
        }
        if (c == '#')
        {
            while (inp.get() != '\n') {}
            inp.get(c); // skip the newline char
        }
    }

    if (c == '+')
    {
        inp.get(c);
        if (c == '=') // +=
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "+="};
        inp.unget(); // move back
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "+"};
    }
    if (c == '-')
    {
        inp.get(c);
        if (c == '>')
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "->"};
        if (c == '=') // -=
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "-="};
        inp.unget(); // if it's not an arrow or -=, the second char is yet to be parsed
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "-"};
    }
    if (c == '*')
    {
        inp.get(c);
        if (c == '=') // *=
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "*="};
        inp.unget(); // move back
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "*"};
    }
    if (c == '/')
    {
        inp.get(c);
        if (c == '=') // /=
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "/="};
        inp.unget(); // move back
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "/"};
    }
    if (c == '^')
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "^"};
    if (c == '(')
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "("};
    if (c == ')')
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = ")"};
    if (c == '{')
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "{"};
    if (c == '}')
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "}"};
    if (c == ';')
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = ";"};
    if (c == ',')
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = ","};
    if (c == '.')
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "."};
    if (c == '=')
    {
        inp.get(c);
        if (c == '=')
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "=="};
        inp.unget();
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "="};
    }
    if (c == '!')
    {
        inp.get(c);
        if (c == '=')
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "!="};
        inp.unget();
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "!"};
    }
    if (c == '<')
    {
        inp.get(c);
        if (c == '=')
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "<="};
        inp.unget();
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "<"};
    }
    if (c == '>')
    {
        inp.get(c);
        if (c == '=')
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = ">="};
        inp.unget();
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = ">"};
    }

    // strings
    if (c == '"')
    {
        std::string s = "";
        while ((c = inp.get()) != '"')
            s.push_back(c);
        return new Lexeme{.LType = Lexeme::LEX_STRING, .LVal = s};
    }

    // numbers
    if (std::isdigit(c))
    {
        std::string num = "";
        num.push_back(c);
        while (isdigit(c = inp.get()) || (c == '.'))
        {
            if (c == '.')
            {
                if (num.find(c) != std::string::npos)
                {
                    // the number already contains a decimal separator,
                    // so this dot is probably meant as a dot operator
                    break;
                }
                num.push_back('.');
            } else num.push_back(c);
        }
        inp.unget();
        if (num[num.length() - 1] == '.') inp.unget(); // terminating dots are also not allowed
        return new Lexeme{.LType = Lexeme::LEX_NUMBER, .LVal = num};
    }

    // identifiers
    if (std::isalnum(c)) // names may not start with underscore
    {
        std::string id = "";
        id.push_back(c);
        while (std::isalnum(c = inp.get()) || c == '_')
            id.push_back(c);
        inp.unget();
        // check if it is a keyword
        for (std::string s : Lexer::keywords) {
            if (id == s) return new Lexeme {.LType = Lexeme::LEX_KW_SYM, .LVal = id};
        }
        return new Lexeme{.LType = Lexeme::LEX_ID, .LVal = id};
    }

    if (inp.eof() || c == std::istream::traits_type::eof() || c == 0)
        return new Lexeme{.LType = Lexeme::LEX_EOF};

    err::throwNonfatal("Invalid token '" + std::string(1, c) + "'", "", line);
    return next();
}