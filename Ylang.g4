grammar Ylang;

code    : lines=line+ EOF ;
line    : 
        'def' 'external' fname=ID '(' (args=ID ':' types=ID)* ')' ':' rettype=ID # externFuncDef
        | 'def' fname=ID '(' (args=ID ':' types=ID)* ')' ':' rettype=ID?
        '{' '\n'? body=expr '}' ('\n' | ';')  # funcDef
        | (name=ID '=' ) e=expr ('\n' | ';')        # varAssign
        | e=expr ('\n' | ';')                       # exprExpr // for simplification    
        ;
expr    : obj=expr ('->') name=ID                   # memberAccess
        | <assoc=right> lhs=expr op='^' rhs=expr    # infixExpr
        | lhs=expr op=('*' | '/' | '%') rhs=expr    # infixExpr
        | lhs=expr op=('+' | '-') rhs=expr          # infixExpr
        | lhs=expr op=('==' | '!=' | '<' | '>' | '<=' | '>=') rhs=expr #infixExpr
        | a=fnatom                                  # atomExpr
        ;

fnatom  : fname=ID '(' args=atom+ ')'               # callExpr
        | a=atom                                    # atomAtom
        ;

atom    : val=('true' | 'false')                    # bool
        | name=ID                                   # variable
        | num=NUMBER                                # number
        | s=STRING                                  # string
        | '(' e=expr ')'                            # parenExpr
        ;

ID      : [a-zA-Z_][a-zA-Z0-9_]* ; 
NUMBER  : [1-9][0-9]*
        | [0-9]+'.'[0-9]+ ;
STRING  : '"' .*? '"' ;
LINEEND : '\n' | ';' ;
WS      : [ \t]+ -> skip ;
