#include "headers/parser.h"
using namespace std;

void Parser::prog() {
    cout << "Parsing stmt..." << '\n';
    stmt();
    cout << "Stmt parsed." << '\n';
    lexer->lex();
    if (lexer->nextToken == NEWLINE) {
        cout << "Parsing prog..." << '\n';
        prog();
        cout << "Prog parsed." << '\n';
    }
}

void Parser::stmt() {
    lexer->lex();
    cout << "Character: " << lexer->nextChar << '\n';
    if (lexer->nextToken == INT_KWD || lexer->nextToken == FLOAT_KWD) {
        cout << "Parsing declaration..." << '\n';
        declaration();
        cout << "Declaration parsed..." << '\n';
    }
    else if (lexer->nextToken == ID) {
        cout << "Parsing assign..." << '\n';
        assign();
        cout << "Assign parsed." << '\n';
    }
    else {
        cout << "ERROR - invalid statement." << '\n';
    }
}

void Parser::declaration() {
    int tok = lexer->nextToken;
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
        cout << "Parsing expr..." << '\n';
        expr();
        cout << "Expr parsed." << '\n';
    }
    else {
        cout << "ERROR - invalid assignment." << '\n';
    }
}

void Parser::expr() {
    cout << "Parsing term..." << '\n';
    term();
    cout << "Term parsed." << '\n';
    cout << "Parsing expr_p..." << '\n';
    expr_p();
    cout << "Expr_p parsed." << '\n';
}

void Parser::expr_p() {
    lexer->lex();
    if (lexer->nextToken == ADD_OP || lexer->nextToken == SUB_OP) {
        lexer->lex();
        cout << "Parsing term..." << '\n';
        term();
        cout << "Term parsed." << '\n';
        cout << "Parsing expr_p..." << '\n';
        expr_p();
        cout << "Expr_p parsed." << '\n';
    }
}

void Parser::term() {
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
        cout << "Parsing factor..." << '\n';
        factor();
        cout << "Factor parsed." << '\n';
        cout << "Parsing term_p..." << '\n';
        term_p();
        cout << "Term_p parsed." << '\n';
    }
    else {
        cout << "ERROR - invalid term." << '\n';
    }
}

void Parser::term_p() {
    lexer->lex();
    if (lexer->nextToken == MULT_OP || lexer->nextToken == DIV_OP || lexer->nextToken == MOD_OP) {
        lexer->lex();
        cout << "Parsing factor..." << '\n';
        factor();
        cout << "Factor parsed." << '\n';
        cout << "Parsing term_p..." << '\n';
        term_p();
        cout << "Term_p parsed." << '\n';
    }
}

void Parser::factor() {
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT) {
        int tok = lexer->nextToken;
        // will probably have to do some stuff with the ast classes here
    }
    else if (lexer->nextToken == LPAREN) {
        lexer->lex();
        if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
            cout << "Parsing expr..." << '\n';
            expr();
            cout << "Expr parsed." << '\n';
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