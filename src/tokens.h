#ifndef TOKENS_H
#define TOKENS_H

enum Token {
    ID=0,
    IF,
    ELIF,
    ELSE,
    FUNC,
    RETURN,
    BOOL_LIT,
    INT_KWD,
    FLOAT_KWD,
    NEWLINE,
    END
};

enum Literal {
    INT_LIT=100,
    FLOAT_LIT
};

enum Operator {
    ADD_OP=200,
    SUB_OP,
    MULT_OP,
    DIV_OP,
    MOD_OP,
    ASSIGN_OP,
    GR,
    GREQ,
    LS,
    LSEQ,
    EQUAL,
    NEQUAL,
    AND,
    OR,
    NOT,
    INVALID_OP
};

enum Control {
    LPAREN=300,
    RPAREN,
    LBRACE,
    RBRACE,
    INVALID_BRACKET
};

#endif