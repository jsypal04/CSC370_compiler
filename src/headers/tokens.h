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
    END,
    INT_LIT=100,
    FLOAT_LIT,
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
    INVALID_OP,
    LPAREN=300,
    RPAREN,
    LBRACE,
    RBRACE,
    INVALID_BRACKET,
    ERROR
};

#endif