#include <iostream>
#include <typeinfo>
#include "headers/parser.h"

int main(int argc, char* argv[]) {
    char* path = argv[1];
    
    Parser parser(path);

    if (parser.lexer->fileNotFound) {
        std::cout << "File \"" << path << "\" not found.\n";
        return -1;
    }

    std::unique_ptr<ProgStmtAST> program = std::move(parser.parse());

    parser.destroyLexer();

    return 0;
}