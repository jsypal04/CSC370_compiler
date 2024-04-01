#include <iostream>
#include "headers/parser.h"
#include "headers/semantics.h"
#include "headers/TACGenerator.h"

int main(int argc, char* argv[]) {
    char* path = argv[1];
    
    Parser parser(path);
    Semantics semantic_analyzer;
    TACGenerator generator;

    if (parser.lexer->fileNotFound) {
        std::cout << "File \"" << path << "\" not found.\n";
        return -1;
    }

    std::unique_ptr<ProgStmtAST> program = std::move(parser.parse());
    program = std::move(semantic_analyzer.traverse_prog(std::move(program)));
    generator.traverse_prog(std::move(program));

    parser.destroyLexer();
    generator.destroyStream();

    return 0;
}