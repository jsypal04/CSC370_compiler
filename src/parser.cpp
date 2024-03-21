#include "headers/parser.h"
using namespace std;

void Parser::prog() {
    stmt();
    lexer->lex();
    if (lexer->nextToken == NEWLINE) {
        prog();
    }
}

void Parser::stmt() {
    lexer->lex();
    if (lexer->nextToken == INT_KWD || lexer->nextToken == FLOAT_KWD) {
        declaration();
    }
    else if (lexer->nextToken == ID) {
        assign();
    }
    else {
        cout << "ERROR - invalid statement." << '\n';
    }
}

void Parser::declaration() {
    Token tok = lexer->nextToken;
    lexer->lex();
    if (lexer->nextToken != ID) {
        cout << "ERROR - invalid declaration." << '\n';
        return;
    }
}

void Parser::assign() {
    lexer->lex();
    if (lexer->nextToken != ASSIGN_OP) {
        cout << "ERROR - invalid assignment." << '\n';
        return;
    }
    lexer->lex();
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
        expr();
    }
    else {
        cout << "ERROR - invalid assignment." << '\n';
    }
}

void Parser::expr() {
    term();
    expr_p();
}

void Parser::expr_p() {
    lexer->lex();
    if (lexer->nextToken == ADD_OP || lexer->nextToken == SUB_OP) {
        lexer->lex();
        term();
        expr_p();
    }
}

void Parser::term() {
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
        factor();
        term_p();
    }
    else {
        cout << "ERROR - invalid term." << '\n';
    }
}

void Parser::term_p() {
    lexer->lex();
    if (lexer->nextToken == MULT_OP || lexer->nextToken == DIV_OP || lexer->nextToken == MOD_OP) {
        lexer->lex();
        factor();
        term_p();
    }
}

void Parser::factor() {
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT) {
        Token tok = lexer->nextToken;
        // will probably have to do some stuff with the ast classes here
    }
    else if (lexer->nextToken == LPAREN) {
        lexer->lex();
        if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
            expr();
        }
        lexer->lex();
        if (lexer->nextToken == RPAREN) {
            // we good
            // probably have to do stuff with ast classes here
        }
        else {
            cout << "ERROR - invalid factor." << '\n';
        }
    }
}