#include <ctype.h>
#include "Lexer.h"

std::ostream& operator<<(std::ostream& os, Lexeme& l)
{
    os << "{ " << l.LType;
    if (l.LType == Lexeme::LEX_EOF)
        os << " }";
    else
        os << " '" << l.LVal << "' }";
    return os;
}

Lexeme *Lexer::next()
{
    char c;
    inp.get(c);
    
    if (inp.eof() || c == std::istream::traits_type::eof() || c == 0)
        return new Lexeme{.LType = Lexeme::LEX_EOF};

    if (std::isspace(c)) // skip whitespace
    {
        while (std::isspace(c = inp.get()))
            ;
    }

    // comments
    if (c == '#')
    {
        while (inp.get() != '\n')
            ;
        inp.get(c); // skip the newline char
    }

    if (c == '+')
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "+"};
    if (c == '-')
    {
        inp.get(c);
        if (c == '>')
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "->"};
        inp.unget(); // if it's not an arrow, the second char is yet to be parsed
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "-"};
    }
    if (c == '*')
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "*"};
    if (c == '/')
        return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "/"};
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
        std::cerr << "Unrecognized token '!'" << std::endl;
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
                    std::cerr << "The number cannot contain two decimal separators" << std::endl;
                    break;
                }
                num.push_back('.');
            } else num.push_back(c);
        }
        inp.unget();
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
        if (id == "def")
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "def"};
        if (id == "do")
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "do"};
        if (id == "external")
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "external"};
        if (id == "type")
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "type"};
        if (id == "if")
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "if"};
        if (id == "else")
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "else"};
        if (id == "switch")
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "switch"};
        if (id == "case")
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "case"};
        if (id == "let")
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "let"};
        if (id == "in")
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "in"};
        if (id == "true")
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "true"};
        if (id == "false")
            return new Lexeme{.LType = Lexeme::LEX_KW_SYM, .LVal = "false"};
        return new Lexeme{.LType = Lexeme::LEX_ID, .LVal = id};
    }

    if (inp.eof() || c == std::istream::traits_type::eof() || c == 0)
        return new Lexeme{.LType = Lexeme::LEX_EOF};

    std::cerr << "Invalid token '" << c << "'" << std::endl;
    return next();
}