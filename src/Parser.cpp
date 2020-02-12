#include "Parser.h"

std::vector<ParseNode*> Parser::parse()
{
    std::vector<ParseNode*> code;
    while (!peek(Lexeme::LEX_EOF))
    {
        if (peekKW("type"))
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

            code.push_back(new TypeDefNode(type_name, members));

        } else if (peekKW("def"))
        {
            eat(); // def

            bool is_external = false;
            if (peekKW("external"))
            {
                is_external = true;
                eat();
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

            if (!is_external)
            {
                
                ParseNode* body = parse_block();     
                code.push_back(new FuncDefNode(rettype, fname, args, body));
            } else {
                
                code.push_back(new ExternFuncDefNode(rettype, fname, args));
            }
        } else
            std::cerr << "Unexpected top-level token. Expected function or type definition, found " << cur_lex->LType << "(" << cur_lex->LVal << ")" << std::endl;
    }
    return code;
}

ParseNode* Parser::parse_block()
{
    if (peekKW("{"))
    {
        eat(); // {
        std::vector<ParseNode*> exprs;
        exprs.push_back(parse_expr());
        while (peekKW(";"))
        {
            eat(); // ;
            if (peekKW("}")) break; // tolerate tailing semicolon
            exprs.push_back(parse_expr());
        }
        expectKW("}");
        return new BlockNode(exprs);
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
        return new LetNode(varName, varVal);
    } else if (peekKW("if"))
    {
        eat(); // if
        ParseNode* cond = parse_expr();
        ParseNode* thenT = parse_expr();
        expectKW("else");
        ParseNode* elseT = parse_expr();
        return new IfNode(cond, thenT, elseT);
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
            ParseNode* case_ = parse_expr();
            cases.push_back(std::pair<ParseNode*, ParseNode*>(rhs, case_));
        }
        expectKW("else");
        expectKW(":");
        ParseNode* elsecase = parse_expr();
        return new SwitchNode(lhs, cases, elsecase);

    } else return parse_mathexpr();
}

ParseNode* Parser::parse_mathexpr()
{
    ParseNode* lhs = parse_cmpexpr();
    if (peekKW("==") || peekKW("!=") || peekKW("<") || peekKW(">") || peekKW("<=") || peekKW(">="))
    {
        std::string op = eat();
        ParseNode* rhs = parse_cmpexpr();
        return new BinOpNode(lhs, op, rhs);
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
        lhs = new BinOpNode(lhs, op, rhs);
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
        lhs = new BinOpNode(lhs, op, rhs);
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
        return new BinOpNode(lhs, "^", rhs);
    }
    return lhs;
}

ParseNode* Parser::parse_unaryexpr()
{
    if (peekKW("-")) {
        eat(); // -
        return new UnaryOpNode("-", parse_dotexpr());
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
        lhs = new CallNode(fname, args);
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
        obj = new MemberAccessNode(obj, member);
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
            return new VariableNode(id);
        eat(); // (
        std::vector<ParseNode*> args;
        if (peekKW(")")) return new CallNode(id, args); // no arguments
        args.push_back(parse_mathexpr());
        while (peekKW(","))
        {
            eat();
            args.push_back(parse_mathexpr());
        }
        expectKW(")");
        return new CallNode(id, args);
    }
    std::cerr << "Unexpected token. Type " << this->cur_lex->LType  << " " << this->cur_lex->LVal << std::endl;
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
    //delete this->cur_lex;
    this->cur_lex = this->l.next();
    return x;
}


std::string Parser::expect(int ltype)
{
    if (!this->peek(ltype)) 
    {
        std::cerr << "Unexpected token. Expected " << ltype << " but found " << this->cur_lex->LType << "(" << this->cur_lex->LVal << ")" << std::endl;
        return "";
    } else return this->eat();
}


void Parser::expectKW(std::string kw)
{
    if (!this->peekKW(kw)) 
        std::cerr << "Unexpected keyword. Expected " << kw  << " but found " << this->cur_lex->LType << " " << this->cur_lex->LVal << std::endl;
    this->eat();   
}