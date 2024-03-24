#include <iostream>
#include <typeinfo>
#include <fstream>
#include "headers/parser.h"

void traverse(std::unique_ptr<StmtAST> program) {
    std::cout << "\n\n\n" << typeid(*program).name() << '\n';
}

int main(int argc, char* argv[]) {
    char* path = argv[1];
    
    Parser parser(path);

    if (parser.lexer->fileNotFound) {
        std::cout << "File \"" << path << "\" not found.\n";
        return -1;
    }

    std::unique_ptr<ProgStmtAST> program = std::move(parser.parse());
    traverse(std::move(program));

    parser.destroyLexer();

    return 0;
}