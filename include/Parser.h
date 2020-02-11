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

    defin :=  'type' typename=ID '{' { membertype=ID membername=ID ';' } '}'
            | 'def' 'external' rettype=ID fname=ID '(' {type=ID arg=ID ',' } ')' # args split by colons
            | 'def' rettype=ID fname=ID '(' {type=ID arg=ID ',' } ')' body=block # args split by colons
    */
    std::vector<ParseNode*> parse();

    /*
    block := expr | '{' expr {';' expr} '}'
    */
    ParseNode* parse_block();

    /*
    expr := ifExpr
            | 'let' name=ID '=' val=expr
            | 'switch' lhs=expr {'case' expr ':' expr} 'else' ':' expr
            | 'if' cond=expr thenT=expr 'else' elseT=expr
    */
    ParseNode* parse_expr();

    /*
    mathExpr := cmpExpr
              | cmpExpr ('==' | '!=' | '<' | '>' | '<=' | '>=') cmpExpr
    */
   ParseNode* parse_mathexpr();

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
    powExpr := dotExpr
            | dotExpr {'^' dotExpr} (right-assoc)
    */
    ParseNode* parse_powexpr();

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