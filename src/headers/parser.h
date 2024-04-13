#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include "lexer.h"
#include "ast.h"

// A class to implement a recursive descent parser for the languages grammar
class Parser {
public:
    // A reference to the lexer object that will be used to provide the stream of tokens to the parser
    Lexer* lexer;

    // Parser Constructor. Currently it only creates a new Lexer refernce
    Parser(char* path) {
        lexer = new Lexer(path);
    }

    ~Parser() {
        delete lexer;
        lexer = nullptr;
    }

    ProgStmtAST* parse() {
        lexer->nextChar = getc(lexer->file);
        lexer->lex();
        std::cout << "Parsing prog..." << '\n';
        ProgStmtAST* program = prog();
        std::cout << "Prog parsed." << '\n';

        return program;
    }

    // These functions each parse a node of the AST and return that node's subtree

    // Parses the program node
    ProgStmtAST* prog();

    // Parses the statement node
    LineStmtAST* stmt();

    // Parses the declaration node
    DeclarationStmtAST* declaration();

    // Parses the assignment node
    AssignStmtAST* assign();

    /********************************************
     * PARSE FUNCTIONS FOR ARITHMETIC EXPRESSIONS
    ********************************************/

    // Parses the expression node
    ExprStmtAST* expr();

    // Parses the expression p node
    Expr_PStmtAST* expr_p();

    // Parses the term node
    TermStmtAST* term();

    // Parses the term p node
    Term_PStmtAST* term_p();

    // Parses the factor node
    FactorStmtAST* factor();

    /*****************************************
     * PARSE FUNCTIONS FOR BOOLEAN EXPRESSIONS
    *****************************************/

    // Parses the boo_expr node
    void bool_expr();

    // Parses the bool_expr_p node
    void bool_expr_p();

    // Parses the bool_term node
    void bool_term();

    // Parses the bool_term_p node
    void bool_term_p();

    // Parses the bool_factor node
    void bool_factor();

    // Parses the relation node
    void relation();

    // Parses the rel_operand node
    void rel_operand();
};

#endif