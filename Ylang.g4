grammar Ylang;

code    : lines=line+ EOF ;
line    : d=defin                                    # definLine
        | s=stmt                                     # stmtLine
        ;
defin   : 
        'def' 'external' rettype=ID fname=ID '(' (type=ID arg=ID)* ')' '\n'*    # externFuncDef
        | 'def' rettype=ID fname=ID '(' (type=ID arg=ID)* ')' body=block '\n'*  # funcDef
        ;
block   : '\n'? stmt                                # stmtBlock
        | '{' '\n'* stmt* '\n'* '}'                 # blockBlock
        ;
stmt    : name=ID '=' e=expr '\n'+                  # varAssign
        | 'return' e=expr '\n'+                     # retExpr
        | e=expr '\n'+                              # exprExpr // for simplification    
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
WS      : [ \t]+ -> skip ;
