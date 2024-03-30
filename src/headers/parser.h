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

    // A method to delete the data in the lexer reference once the parser is finished
    void destroyLexer() {
        delete lexer;
    }

    std::unique_ptr<ProgStmtAST> parse() {
        lexer->nextChar = getc(lexer->file);
        lexer->lex();
        //std::cout << "Parsing prog..." << '\n';
        std::unique_ptr<ProgStmtAST> program = std::move(prog());
        //std::cout << "Prog parsed." << '\n';

        return program;
    }

    // These functions each parse a node of the AST and return that node's subtree

    // Parses the program node
    std::unique_ptr<ProgStmtAST> prog();

    // Parses the statement node
    std::unique_ptr<LineStmtAST> stmt();

    // Parses the declaration node
    std::unique_ptr<DeclarationStmtAST> declaration();

    // Parses the assignment node
    std::unique_ptr<AssignStmtAST> assign();

    // Parses the expression node
    std::unique_ptr<ExprStmtAST> expr();

    // Parses the expression p node
    std::unique_ptr<Expr_PStmtAST> expr_p();

    // Parses the term node
    std::unique_ptr<TermStmtAST> term();

    // Parses the term p node
    std::unique_ptr<Term_PStmtAST> term_p();

    // Parses the factor node
    std::unique_ptr<FactorStmtAST> factor();
};

#endif