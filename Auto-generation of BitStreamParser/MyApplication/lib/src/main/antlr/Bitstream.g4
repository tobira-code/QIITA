grammar Bitstream;

// parser grammar
compilationUnit
    :   syntaxList? EOF
    ;

syntaxList
    :   syntax
    |   syntaxList syntax
    ;

syntax
    :   Identifier '{' elemntList '}'
    ;

elemntList
    :   element
    |   elemntList element
    ;

element
    :   Identifier DecimalConstant
    ;

// lexer grammar
Whitespace
    :   [ \t\r\n]+
        -> skip
    ;

Identifier
    :   Nondigit
        (   Nondigit
        |   Digit
        )*
    ;

DecimalConstant
    :   NonzeroDigit Digit*
    ;

fragment
Nondigit
    :   [a-zA-Z_]
    ;

fragment
Digit
    :   [0-9]
    ;

fragment
NonzeroDigit
    :   [1-9]
    ;

