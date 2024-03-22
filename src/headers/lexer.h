#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <stdio.h>
#include "tokens.h"

// A class to provide a stream of tokens to the parser
class Lexer {
public:
    char nextChar; // a variable to store each char read from the source code
    Token nextToken; // a variable to store the token of a given lexeme
    std::string lexeme; // a variable to store a lexeme
    FILE *file; // a variabel to point to a file stream
    char opSymbols[10] = {'+', '=', '-', '*', '/', '%', '<', '>', '&', '|'};

    Lexer() {
        file = fopen("code.txt", "r");
    }

    // A method to check if a char is a operator symbol
    bool isOpSymbol(char symbol);

    // A method to check if a char is a bracket character, either a brace or parenthesis
    bool isBracket(char symbol);

    // A method to skip spaces between lexemes 
    void skipSpace();

    // A method to get the token of a given lexeme
    Token lookupToken(std::string lexeme);

    // A method to get the operator token for a 1 character operator
    Token lookupOneChar(std::string lexeme);

    // A method to get the operator token for a 2 character operator
    Token lookupTwoChar(std::string op);

    // A method to get the control token for a bracket character
    Token lookupBracket(char bracket);

    // A method to get the nextToken and lexeme from a source file
    void lex();
};

#endif