#include <iostream>
#include "headers/parser.h"
#include "headers/semantics.h"
#include "headers/TACGenerator.h"

int main(int argc, char* argv[]) {
    char* path = argv[1];
    
    Parser* parser = new Parser(path);
    Semantics semantic_analyzer;
    TACGenerator gen("a.tac");

    if (parser->lexer->fileNotFound) {
        std::cout << "File \"" << path << "\" not found.\n";
        return -1;
    }

    ProgStmtAST* program = parser->parse();
    delete parser;
    parser = nullptr;

    semantic_analyzer.traverse_prog(program);
    gen.traverse_prog(program);
    gen.destroyStream();

    delete program;
    program = nullptr;
    return 0;
}