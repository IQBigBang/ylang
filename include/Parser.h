#ifndef PARSER_H_
#define PARSER_H_

#include "Lexer.h"
#include "Tree.h"
#include <vector>

class Parser
{
public:
    Parser(Lexer& l) : l(l) {
        this->cur_lex = l.next(); // init first lexeme
    }

    /*
    Main parse function

    code := {defin} EOF;

    defin :=  'include' filename=ID
            | 'type' typename=ID '{' { membertype=ID membername=ID ';' } '}'
            | 'fn' attributes rettype=ID fname=ID '(' {type=ID arg=ID ',' } ')' body=block # args split by colons
    */
    std::vector<ParseNode*> parse();

    /*
    block := expr | '{' expr {';' expr} '}'
    */
    ParseNode* parse_block();

    /*
    expr := mathExpr
            | 'let' name=ID '=' val=expr
            | 'switch' lhs=expr {'case' expr ':' block} 'else' ':' block
            | 'if' cond=expr thenT=block 'else' elseT=block
            | 'while' cond=expr body=block
            | val=mathExpr '=' val=mathExpr
            | mathExpr
    */
    ParseNode* parse_expr();

    /*
    mathExpr := boolExpr
              | boolExpr {('and' | 'or') boolExpr}
    */
    ParseNode* parse_mathexpr();

    /*
    boolExpr := cmpExpr
              | cmpExpr ('==' | '!=' | '<' | '>' | '<=' | '>=') cmpExpr
    */
   ParseNode* parse_boolexpr();

    /*
    cmpExpr := termExpr
            | termExpr {('+' | '-') termExpr}
    */
    ParseNode* parse_cmpexpr();

    /*
    termExpr := powExpr
            | powExpr {('*' | '/') powExpr}
    */
    ParseNode* parse_termexpr();

    /*
    powExpr := unaryExpr
            | unaryExpr {'^' unaryExpr} (right-assoc)
    */
    ParseNode* parse_powexpr();

    /*
    unaryExpr := ['-'] dotExpr
    */
    ParseNode* parse_unaryexpr();

    /*
    dotExpr := memberExpr
             | memberExpr {'.' ID '(' {expr ','} ')'} ')'}
    */
   ParseNode* parse_dotexpr();

    /*
    memberExpr := atom
            | atom {'->' ID}
    */
    ParseNode* parse_memberexpr();

    /*
    atom := 'true' | 'false' | NUMBER | STRING
        | '(' expr ')' | ID '(' {expr ','} ')' | ID
    */
    ParseNode* parse_atom();


private:
    Lexer& l;
    Lexeme* cur_lex;

    bool peek(int ltype);
    bool peekKW(std::string kw);
    std::string expect(int ltype);
    void expectKW(std::string kw);
    std::string eat();
    // if there's a stylistic (non-grammar) newline, skip it
    void possible_nl();
};

#endif // PARSER_H_