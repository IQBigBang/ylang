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
                members.push_back(std::pair<std::string, std::string>(expect(Lexeme::LEX_ID), expect(Lexeme::LEX_ID)));
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
            while (!peekKW(")"))
            {
                args.push_back(std::pair<std::string, std::string>(expect(Lexeme::LEX_ID), expect(Lexeme::LEX_ID)));
            }
            eat(); // ")"

            if (!is_external)
            {
                
                ParseNode* body = parse_expr();     
                code.push_back(new FuncDefNode(rettype, fname, args, body));
            } else {
                
                code.push_back(new ExternFuncDefNode(rettype, fname, args));
            }
        } else
            std::cerr << "Unexpected top-level token. Expected function or type definition" << std::endl;
    }
    return code;
}

ParseNode* Parser::parse_expr()
{
    if (!peekKW("let") && !peekKW("do"))
        return parse_letinexpr();
    if (peekKW("do"))
    {
        eat(); // do
        std::vector<ParseNode*> body;
        expectKW("{");
        while (!peekKW("}"))
            body.push_back(parse_expr());
        expectKW("}");
        return new DoNode(body);
    }
    expectKW("let");
    std::string name = expect(Lexeme::LEX_ID);
    expectKW("=");
    ParseNode* val = parse_letinexpr();
    expectKW("in");
    ParseNode* e = parse_expr();
    
    return new LetInNode(name, val, e);
}

ParseNode* Parser::parse_letinexpr()
{
    if (!peekKW("switch"))
        return parse_switchexpr();
    expectKW("switch");
    ParseNode* lhs = parse_ifexpr();
    
    // vector of <rhs, case>
    std::vector<std::pair<ParseNode*, ParseNode*>> cases;
    while (peekKW("case"))
    {
        eat(); // case
        ParseNode* rhs = parse_ifexpr();
        ParseNode* case_ = parse_switchexpr();
        cases.push_back(std::pair<ParseNode*, ParseNode*>(rhs, case_));
    }
    expectKW("else");
    ParseNode* elsecase = parse_switchexpr();
    return new SwitchNode(lhs, cases, elsecase);
}

ParseNode* Parser::parse_switchexpr()
{
    if (!peekKW("if"))
        return parse_ifexpr();
    expectKW("if");
    ParseNode* cond = parse_ifexpr();
    ParseNode* thenT = parse_expr();
    expectKW("else");
    ParseNode* elseT = parse_expr();
    return new IfNode(cond, thenT, elseT);
}

ParseNode* Parser::parse_ifexpr()
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
    ParseNode* lhs = parse_memberexpr();
    if (peekKW("^"))
    {
        eat();
        ParseNode* rhs = parse_powexpr();
        return new BinOpNode(lhs, "^", rhs);
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
        ParseNode* e = parse_expr();
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
        while (!peekKW(")"))
            args.push_back(parse_expr());
        eat(); // )
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
        std::cerr << "Unexpected token. Expected " << ltype << " but found " << this->cur_lex->LType << std::endl;
        return "";
    } else return this->eat();
}


void Parser::expectKW(std::string kw)
{
    if (!this->peekKW(kw)) 
        std::cerr << "Unexpected keyword. Expected " << kw  << " but found " << this->cur_lex->LType << " " << this->cur_lex->LVal << std::endl;
    this->eat();   
}