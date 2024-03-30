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

        // TEST CODE
        // lexer->nextChar = getc(lexer->file);
        // do {
        //     lexer->lex();
        //     std::cout << "Token: " << lexer->nextToken << ", Lexeme: " << lexer->lexeme << '\n';
        // } while (lexer->nextChar != EOF);
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

    std::unique_ptr<ProgStmtAST> prog();

    std::unique_ptr<LineStmtAST> stmt();

    std::unique_ptr<DeclarationStmtAST> declaration();

    std::unique_ptr<AssignStmtAST> assign();

    std::unique_ptr<ExprStmtAST> expr();

    std::unique_ptr<Expr_PStmtAST> expr_p();

    std::unique_ptr<TermStmtAST> term();

    std::unique_ptr<Term_PStmtAST> term_p();

    std::unique_ptr<FactorStmtAST> factor();
};

#endif