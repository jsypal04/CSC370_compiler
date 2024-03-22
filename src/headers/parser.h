#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include "lexer.h"

// A class to implement a recursive descent parser for the languages grammar
class Parser {
public:
    // A reference to the lexer object that will be used to provide the stream of tokens to the parser
    Lexer* lexer;

    // Parser Constructor. Currently it only creates a new Lexer refernce
    Parser() {
        lexer = new Lexer();

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

    void parse() {
        lexer->nextChar = getc(lexer->file);
        lexer->lex();
        std::cout << "Parsing prog..." << '\n';
        prog();
        std::cout << "Prog parsed." << '\n';
    }

    // These functions have not yet been implemented
    void prog();

    void stmt();

    void declaration();

    void assign();

    void expr();

    void expr_p();

    void term();

    void term_p();

    void factor();
};

#endif