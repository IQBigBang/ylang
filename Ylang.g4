grammar Ylang;

code    : lines=line+ EOF ;
line    : d=defin                                    # definLine
        ;
defin   : 
        'def' 'external' rettype=ID fname=ID '(' (type=ID arg=ID)* ')' '\n'*    # externFuncDef
        | 'def' rettype=ID fname=ID '(' (type=ID arg=ID)* ')' '\n'? body=expr '\n'*  # funcDef
        ;

expr    : obj=expr ('->') name=ID                   # memberAccess
        | <assoc=right> lhs=expr op='^' rhs=expr    # infixExpr
        | lhs=expr op=('*' | '/' | '%') rhs=expr    # infixExpr
        | lhs=expr op=('+' | '-') rhs=expr          # infixExpr
        | lhs=expr op=('==' | '!=' | '<' | '>' | '<=' | '>=') rhs=expr #infixExpr
        | 'if' cond=expr '\n'? thenT=expr '\n'? 'else' '\n'? elseT=expr   # ifExpr
        | 'switch' lhs=expr ('\n'? 'case' expr '\n'? expr)* '\n'? 'else' '\n'? expr # switchExpr
        | 'let' name=ID '=' val=expr 'in' '\n'? e=expr  # letInExpr
        | a=fnatom                                  # atomExpr
        ;

fnatom  : fname=ID '(' args=expr+ ')'               # callExpr
        | a=atom                                    # atomAtom
        ;

atom    : val=('true' | 'false')                    # bool
        | name=ID                                   # variable
        | num=NUMBER                                # number
        | s=STRING                                  # string
        | '(' e=expr ')'                            # parenExpr
        ;

ID      : [a-zA-Z_][a-zA-Z0-9_]* ; 
NUMBER  : [0-9]+
        | [0-9]+'.'[0-9]+ ;
STRING  : '"' .*? '"' ;
WS      : [ \t]+ -> skip ;
