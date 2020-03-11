#include "Parser.h"
#include "Errors.h"

std::vector<ParseNode*> Parser::parse()
{
    std::vector<ParseNode*> code;
    while (!peek(Lexeme::LEX_EOF))
    {
        if (peekKW("include")) {
            eat(); // include
            std::string module_name = expect(Lexeme::LEX_ID);

            code.push_back(new IncludeNode(l.getLine(), module_name));
        } else if (peekKW("type"))
        {
            eat(); // type
            std::string type_name = expect(Lexeme::LEX_ID);

            std::vector<std::pair<std::string, std::string>> members;
            expectKW("{");
            while (!peekKW("}"))
            {
                members.push_back(std::pair<std::string, std::string>(expect(Lexeme::LEX_ID), expect(Lexeme::LEX_ID)));
                expectKW(";");
            }
            eat(); // }

            code.push_back(new TypeDefNode(l.getLine(), type_name, members));

        } else if (peekKW("fn"))
        {
            eat(); // fn

            FuncAttributes attrs = {false, false};
            if (peekKW("(")) // attributes
            {
                do {
                    eat();
                    std::string attr = expect(Lexeme::LEX_ID);
                    if (attr == "external")
                        attrs.External = true;
                    else if (attr == "unmangled")
                        attrs.Unmangled = true;
                    else err::throwNonfatal("Invaldid function attribute", "Unrecognized function attribute '" + attr + "'", l.getLine());
                } while (peekKW(","));
                expectKW(")");
            }
            
            std::string rettype = expect(Lexeme::LEX_ID);
            std::string fname = expect(Lexeme::LEX_ID);

            expectKW("(");
            // vector of <argtype, argname>

            std::vector<std::pair<std::string, std::string>> args;
            if (!peekKW(")")) { // if next is ')', there are zero arguments
                args.push_back(std::pair<std::string, std::string>(expect(Lexeme::LEX_ID), expect(Lexeme::LEX_ID)));
                while (peekKW(","))
                {
                    eat(); // ,
                    args.push_back(std::pair<std::string, std::string>(expect(Lexeme::LEX_ID), expect(Lexeme::LEX_ID)));
                }
            }
            expectKW(")");

            if (attrs.External)
            {
                code.push_back(new FuncDefNode(l.getLine(), attrs, rettype, fname, args, nullptr));
            } else {   
                code.push_back(new FuncDefNode(l.getLine(), attrs, rettype, fname, args, parse_block()));
            }
        } else {
            err::throwNonfatal(
                "Unexpected token", 
                "Found unexpected " + LTypeToStr(this->cur_lex->LType) + " '" + this->cur_lex->LVal + "'",
                this->l.getLine());
            eat();
        }
    }
    return code;
}

ParseNode* Parser::parse_block()
{
    if (peekKW("{"))
    {
        eat(); // {
        std::vector<ParseNode*> exprs;
        if (peekKW("}")) { // empty block
            eat();
            return new BlockNode(l.getLine(), exprs);
        }
        exprs.push_back(parse_expr());
        while (peekKW(";"))
        {
            eat(); // ;
            if (peekKW("}")) break; // tolerate tailing semicolon
            exprs.push_back(parse_expr());
        }
        expectKW("}");
        return new BlockNode(l.getLine(), exprs);
    } else return parse_expr();
}

ParseNode* Parser::parse_expr()
{
    if (peekKW("let"))
    {
        eat(); // let
        std::string varName = expect(Lexeme::LEX_ID);
        expectKW("=");
        ParseNode* varVal = parse_expr();
        return new LetNode(l.getLine(), varName, varVal);
    } else if (peekKW("if"))
    {
        eat(); // if
        ParseNode* cond = parse_expr();
        ParseNode* thenT = parse_block();
        if (!peekKW("else")) // implicit else
        {
            std::vector<ParseNode*> empty_vector;
            BlockNode* empty_block = new BlockNode(l.getLine(), empty_vector);
            return new IfNode(l.getLine(), cond, thenT, empty_block);
        }
        expectKW("else");
        ParseNode* elseT = parse_block();
        return new IfNode(l.getLine(), cond, thenT, elseT);
    } else if (peekKW("switch"))
    {
        eat(); // switch
        ParseNode* lhs = parse_expr();

        // vector of <rhs, case>
        std::vector<std::pair<ParseNode*, ParseNode*>> cases;
        while (peekKW("case"))
        {
            eat(); // case
            ParseNode* rhs = parse_expr();
            expectKW(":");
            ParseNode* case_ = parse_block();
            cases.push_back(std::pair<ParseNode*, ParseNode*>(rhs, case_));
        }
        expectKW("else");
        expectKW(":");
        ParseNode* elsecase = parse_block();
        return new SwitchNode(l.getLine(), lhs, cases, elsecase);

    } else if (peekKW("while")) {
        eat(); // while
        return new WhileNode(l.getLine(), parse_expr(), parse_block());
    } else {
        ParseNode* expr = parse_mathexpr();
        if (peekKW("=")) {
            eat(); // =
            ParseNode* value = parse_expr();
            return new AssignNode(l.getLine(), expr, value);
        } else return expr;
    }
}

ParseNode* Parser::parse_mathexpr()
{
    ParseNode* lhs = parse_boolexpr();
    while (peekKW("and") || peekKW("or"))
    {
        std::string op = eat();
        ParseNode* rhs = parse_boolexpr();
        lhs = new BinOpNode(l.getLine(), lhs, op, rhs);
    }
    return lhs;
}

ParseNode* Parser::parse_boolexpr()
{
    ParseNode* lhs = parse_cmpexpr();
    if (peekKW("==") || peekKW("!=") || peekKW("<") || peekKW(">") || peekKW("<=") || peekKW(">="))
    {
        std::string op = eat();
        ParseNode* rhs = parse_cmpexpr();
        return new BinOpNode(l.getLine(), lhs, op, rhs);
    }
    return lhs;
}

ParseNode* Parser::parse_cmpexpr()
{
    ParseNode* lhs = parse_termexpr();
    while (peekKW("+") || peekKW("-"))
    {
        std::string op = eat();
        ParseNode* rhs = parse_termexpr();
        lhs = new BinOpNode(l.getLine(), lhs, op, rhs);
    }
    return lhs;
}

ParseNode* Parser::parse_termexpr()
{
    ParseNode* lhs = parse_powexpr();
    while (peekKW("*") || peekKW("/"))
    {
        std::string op = eat();
        ParseNode* rhs = parse_powexpr();
        lhs = new BinOpNode(l.getLine(), lhs, op, rhs);
    }
    return lhs;
}

ParseNode* Parser::parse_powexpr()
{
    ParseNode* lhs = parse_unaryexpr();
    if (peekKW("^"))
    {
        eat();
        ParseNode* rhs = parse_powexpr();
        return new BinOpNode(l.getLine(), lhs, "^", rhs);
    }
    return lhs;
}

ParseNode* Parser::parse_unaryexpr()
{
    if (peekKW("-") || peekKW("!")) {
        std::string op = eat();
        return new UnaryOpNode(l.getLine(), op, parse_unaryexpr());
    }
    return parse_dotexpr();
}

ParseNode* Parser::parse_dotexpr()
{
    ParseNode* lhs = parse_memberexpr();
    //lhs->print();
    while (peekKW("."))
    {
        eat(); // .
        std::string fname = expect(Lexeme::LEX_ID);
        expectKW("(");
        std::vector<ParseNode*> args;
        args.push_back(lhs); // first arg is the thing before the dot
        if (!peekKW(")")) {
            args.push_back(parse_mathexpr());
            while (peekKW(","))
            {
                eat();
                args.push_back(parse_mathexpr());
            }
        }
        expectKW(")");
        lhs = new CallNode(l.getLine(), fname, args);
    }
    return lhs;
}

ParseNode* Parser::parse_memberexpr()
{
    ParseNode* obj = parse_atom();
    while (peekKW("->"))
    {
        eat();
        std::string member = expect(Lexeme::LEX_ID);
        obj = new MemberAccessNode(l.getLine(), obj, member);
    }
    return obj;
}

ParseNode* Parser::parse_atom()
{
    if (peekKW("true"))
    {
        eat();
        return new BoolNode(true);
    }
    if (peekKW("false"))
    {
        eat();
        return new BoolNode(false);
    }
    if (peek(Lexeme::LEX_NUMBER))
    {
        std::string num = eat();
        return new NumberNode(std::stod(num));
    }
    if (peek(Lexeme::LEX_STRING))
    {
        return new StringNode(eat());
    }
    if (peekKW("("))
    {
        eat(); // (
        ParseNode* e = parse_mathexpr();
        expectKW(")");
        return e;
    }
    if (peek(Lexeme::LEX_ID))
    {
        std::string id = eat();
        if (!peekKW("("))
            return new VariableNode(l.getLine(), id);
        eat(); // (
        std::vector<ParseNode*> args;
        if (peekKW(")")) return new CallNode(l.getLine(), id, args); // no arguments
        args.push_back(parse_mathexpr());
        while (peekKW(","))
        {
            eat();
            args.push_back(parse_mathexpr());
        }
        expectKW(")");
        return new CallNode(l.getLine(), id, args);
    }

    err::throwNonfatal(
        "Unexpected token", 
        "Found unexpected " + LTypeToStr(this->cur_lex->LType) + " '" + this->cur_lex->LVal + "'",
        this->l.getLine());
    // try to save it by returning a random node
    eat();
    return new NumberNode(0);
}

bool Parser::peek(int ltype)
{
    return this->cur_lex->LType == ltype;
}


bool Parser::peekKW(std::string kw)
{
    return this->cur_lex->LType == Lexeme::LEX_KW_SYM && this->cur_lex->LVal == kw;
}


std::string Parser::eat()
{
    std::string x = this->cur_lex->LVal;
    delete this->cur_lex;
    this->cur_lex = this->l.next();
    return x;
}


std::string Parser::expect(int ltype)
{
    if (!this->peek(ltype)) 
    {
        err::throwNonfatal(
            "Unexpected token", 
            "Expected " + LTypeToStr(ltype) + " but found " + LTypeToStr(this->cur_lex->LType) + " '" + this->cur_lex->LVal + "'",
            this->l.getLine());
    }
    return this->eat();
}


void Parser::expectKW(std::string kw)
{
    if (!this->peekKW(kw))
    { 
        err::throwNonfatal(
            "Unexpected token", 
            "Expected keyword " + kw + " but found " + LTypeToStr(this->cur_lex->LType) + " '" + this->cur_lex->LVal + "'",
            this->l.getLine());
    }
    this->eat();   
}