#include <iostream>
#include <typeinfo>
#include "headers/parser.h"

void traverse_prog(std::unique_ptr<ProgStmtAST> program);

int main() {

    Parser parser;

    std::unique_ptr<ProgStmtAST> program = std::move(parser.parse());
    std::cout << "\n\n\n\n" << typeid(*program).name() << '\n';

    parser.destroyLexer();

    return 0;
}