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
    defin := 'def' 'external' rettype=ID fname=ID '(' {type=ID arg=ID} ')'
            | 'def' rettype=ID fname=ID '(' {type=ID arg=ID} ')' body=expr
    */
    std::vector<ParseNode*> parse();

    /*
    expr := letInExpr
          | 'let' name=ID '=' val=letInExpr 'in' expr
    */
    ParseNode* parse_expr();

    /*
    letInExpr := switchExpr
            | 'switch' lhs=ifExpr {'case' ifExpr switchExpr} 'else' switchExpr
    */
    ParseNode* parse_letinexpr();

    /*
    switchExpr := ifExpr
            |  'if' cond=ifExpr thenT=expr 'else' elseT=expr
    */
    ParseNode* parse_switchexpr();

    /*
    ifExpr := cmpExpr
            | cmpExpr ('==' | '!=' | '<' | '>' | '<=' | '>=') cmpExpr
    */
   ParseNode* parse_ifexpr();

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
    powExpr := memberExpr
            | memberExpr {'^' memberExpr} (right-assoc)
    */
    ParseNode* parse_powexpr();

    /*
    memberExpr := atom
            | atom {'->' ID}
    */
    ParseNode* parse_memberexpr();

    /*
    atom := 'true' | 'false' | NUMBER | STRING
        | '(' expr ')' | ID '(' {expr} ')' | ID
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