#include <iostream>
#include <typeinfo>
#include "headers/parser.h"

void traverse_prog(std::unique_ptr<StmtAST> program) {
    std::cout << "\n\n\n" << typeid(*program).name() << '\n';
}

int main(int argc, char* argv[]) {
    char* path = argv[1];
    Parser parser(path);

    std::unique_ptr<ProgStmtAST> program = std::move(parser.parse());

    parser.destroyLexer();

    return 0;
}