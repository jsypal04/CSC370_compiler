#include <iostream>
#include "headers/parser.h"
#include "headers/semantics.h"
#include "headers/TACGenerator.h"

int main(int argc, char* argv[]) {
    char* path = argv[1];
    
    Parser parser(path);
    Semantics semantic_analyzer;

    if (parser.lexer->fileNotFound) {
        std::cout << "File \"" << path << "\" not found.\n";
        return -1;
    }

    ProgStmtAST* program = parser.parse();
    semantic_analyzer.traverse_prog(program);

    parser.destroyLexer();

    return 0;
}