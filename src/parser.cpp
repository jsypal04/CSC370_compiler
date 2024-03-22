#include "headers/parser.h"
#include "headers/ast.h"
using namespace std;

void Parser::prog() {
    cout << "Parsing stmt..." << '\n';
    stmt();
    cout << "Stmt parsed." << '\n';
    if (lexer->nextToken == NEWLINE) {
        lexer->lex();
        cout << "Parsing prog..." << '\n';
        prog();
        cout << "Prog parsed." << '\n';
    }
}

void Parser::stmt() {
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
    Token tok = lexer->nextToken;
    cout << "Type: " << lexer->lexeme;
    lexer->lex();
    cout << ", variable: " << lexer->lexeme << "\n";
    if (lexer->nextToken != ID) {
        cout << "ERROR - invalid declaration." << '\n';
        return;
    }
    else {
        lexer->lex();
    }
}

void Parser::assign() {
    cout << "Lefthand side: " << lexer->lexeme << '\n';
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
    //cout << "\n\nEXPR_P TOKEN: " << lexer->nextToken << ", LEXEME: " << lexer->lexeme << "\n\n";
    if (lexer->nextToken == ADD_OP || lexer->nextToken == SUB_OP) {
        lexer->lex();
        cout << "Parsing term..." << '\n';
        term();
        cout << "Term parsed." << '\n';
        cout << "Parsing expr_p..." << '\n';
        expr_p();
        cout << "Expr_p parsed." << '\n';
        // may need another lexer->lex() here
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
    if (lexer->nextToken == MULT_OP || lexer->nextToken == DIV_OP || lexer->nextToken == MOD_OP) {
        lexer->lex();
        cout << "Parsing factor..." << '\n';
        factor();
        cout << "Factor parsed." << '\n';
        cout << "Parsing term_p..." << '\n';
        term_p();
        cout << "Term_p parsed." << '\n';
        // may need another lexer->lex() here
    }
}

void Parser::factor() {
    if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT) {
        Token tok = lexer->nextToken;
        cout << "Factor: " << lexer->lexeme << '\n';
        lexer->lex();
        // will probably have to do some stuff with the ast classes here
    }
    else if (lexer->nextToken == LPAREN) {
        lexer->lex();
        if (lexer->nextToken == ID || lexer->nextToken == INT_LIT || lexer->nextToken == FLOAT_LIT || lexer->nextToken == LPAREN) {
            cout << "Parsing expr..." << '\n';
            expr();
            cout << "Expr parsed." << '\n';
        }
        else {
            cout << "ERROR - invalid factor.\n";
        }
        if (lexer->nextToken == RPAREN) {
            lexer->lex();
            // we good
            // probably have to do stuff with ast classes here
        }
        else {
            cout << "ERROR - invalid factor." << '\n';
        }
    }
}