#include <iostream>
#include <ctype.h>
#include "headers/lexer.h"
using namespace std;

bool Lexer::isOpSymbol(char symbol) {
    // get the length of opSymbols
    int length = sizeof(opSymbols) / sizeof(opSymbols[0]);
    // loop through it and if symbol is in the array return true
    for (int i = 0; i < length; i++) {
        if (opSymbols[i] == symbol) {
            return true;
        }
    }
    return false; // otherwise return false
}

bool Lexer::isBracket(char symbol) {
    return symbol == '(' || symbol == ')' || symbol == '{' || symbol == '}';
}

void Lexer::skipSpace() {
    while (nextChar == ' ') {
        nextChar = getc(file);
    }
}

Token Lexer::lookupToken(string lexeme) {
    if (lexeme == "if") {
        return IF;
    }
    else if (lexeme == "elif") {
        return ELIF;
    }
    else if (lexeme == "else") {
        return ELSE;
    }
    else if (lexeme == "func") {
        return FUNC;
    }
    else if (lexeme == "return") {
        return RETURN;
    }
    else if (lexeme == "true" || lexeme == "false") {
        return BOOL_LIT;
    }
    else if (lexeme == "int") {
        return INT_KWD;
    }
    else if (lexeme == "float") {
        return FLOAT_KWD;
    }
    else {
        return ID;
    }
}

Token Lexer::lookupOneChar(string op) {
    char opc;
    if (op.size() != 1) {
        cout << "ERROR - something went wrong with the operator\n";
        return INVALID_OP;
    }

    opc = op[0];
    switch (opc) {
        case '+':
            return ADD_OP;
        case '-':
            return SUB_OP;
        case '*':
            return MULT_OP;
        case '/':
            return DIV_OP;
        case '%':
            return MOD_OP;
        case '>':
            return GR;
        case '<':
            return LS;
        case '=':
            return ASSIGN_OP;
        case '&':
            return AND;
        case '|':
            return OR;
        default:
            cout << "ERROR - invalid operator\n";
        return INVALID_OP;
    }
}

Token Lexer::lookupTwoChar(string op) {
    if (op.size() != 2) {
        cout << "ERROR - something went wrong with the operator\n";
        return INVALID_OP;
    }
    else if (op == ">=") {
        return GREQ;
    }
    else if (op == "<=") {
        return LSEQ;
    }
    else if (op == "==") {
        return EQUAL;
    }
    else if (op == "!=") {
        return NEQUAL;
    }
    else {
        cout << "ERROR - invalid operator\n";
        return INVALID_OP;
    }
}

Token Lexer::lookupBracket(char bracket) {
    switch (bracket)
    {
    case '(':
        return LPAREN;
        break;
    case ')':
        return RPAREN;
        break;
    case '{':
        return LBRACE;
        break;
    case '}':
        return RBRACE;
        break;
    default:
        cout << "ERROR - invalid bracket\n";
        return INVALID_BRACKET;
    }
}

void Lexer::lex() {
    // reset the lexeme
    lexeme = "";
    skipSpace();
    // id state
    if (isalpha(nextChar)) {
        lexeme += nextChar;
        nextChar = getc(file);
        while (isalnum(nextChar)) {
            lexeme += nextChar;
            nextChar = getc(file);
        }
        // if space, the lexeme is complete
        nextToken = lookupToken(lexeme);
    }
    // number state
    else if (isdigit(nextChar)) {
        bool isint = true;
        lexeme += nextChar;
        nextChar = getc(file);
        while (isdigit(nextChar) || nextChar == '.') {
            if (nextChar == '.') {
                isint = false;
            }
            lexeme += nextChar;
            nextChar = getc(file);
        }
        if (isint) {
            nextToken = INT_LIT;
        }
        else if (!isint) {
            nextToken = FLOAT_LIT;
        }
    }
    // float state
    else if (nextChar == '.') {
        lexeme += nextChar;
        nextChar = getc(file);
        while (isdigit(nextChar)) {
            lexeme += nextChar;
            nextChar = getc(file);
        }
        nextToken = FLOAT_LIT;
    }
    // symbol state
    else if (isOpSymbol(nextChar)) {
        lexeme += nextChar;
        nextChar = getc(file);
        if (isOpSymbol(nextChar)) {
            lexeme += nextChar;
            nextChar = getc(file);
            nextToken = lookupTwoChar(lexeme);
        }
        else if (!isOpSymbol(nextChar)) {
            nextToken = lookupOneChar(lexeme);
        }
        else {
            cout << "ERROR - invalid operator\n";
        }
    }
    // not state
    else if (nextChar == '!') {
        lexeme += nextChar;
        nextToken = NOT;
        nextChar = getc(file);
    }
    // bracket state
    else if (isBracket(nextChar)) {
        lexeme += nextChar;
        nextToken = lookupBracket(nextChar);
        nextChar = getc(file);
    }
    // newline state
    else if (nextChar == '\n') {
        lexeme += nextChar;
        nextToken = NEWLINE;
        nextChar = getc(file);
    }
    // end state
    else if (nextChar == EOF) {
        lexeme += '$';
        nextToken = END;
    }
    else {
        cout << "ERROR - invalid token: " << (int)nextChar << '\n';
    }
}
//test Sujar