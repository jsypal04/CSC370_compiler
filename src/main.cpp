#include <iostream>
#include "lexer.h"
using namespace std;

int main() {

    Lexer lexer;
    lexer.nextChar = getc(lexer.file);
    do {
        lexer.lex();
        cout << "Token: " << lexer.nextToken << ", Lexeme: " << lexer.lexeme << '\n';
    } while (lexer.nextChar != EOF);

    return 0;
}