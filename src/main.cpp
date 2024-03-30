#include <iostream>
#include <typeinfo>
#include "headers/parser.h"
#include "headers/semantics.h"

int main(int argc, char* argv[]) {
    char* path = argv[1];
    
    Parser parser(path);
    Semantics semantic_analyzer;

    if (parser.lexer->fileNotFound) {
        std::cout << "File \"" << path << "\" not found.\n";
        return -1;
    }

    std::unique_ptr<ProgStmtAST> program = std::move(parser.parse());
    semantic_analyzer.traverse_prog(std::move(program));

    parser.destroyLexer();

    return 0;
}