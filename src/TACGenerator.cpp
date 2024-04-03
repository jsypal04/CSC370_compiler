#include <iostream>
#include "headers/TACGenerator.h"

void TACGenerator::traverse_prog(ProgStmtAST* prog) {
    std::cout << "Entering program...\n";
    traverse_stmt(std::move(prog->stmt));
    if (prog->prog != nullptr) {
        traverse_prog(std::move(prog->prog));
    }
}

void TACGenerator::traverse_stmt(LineStmtAST* stmt) {
    std::cout << "Entering Statement...\n";
    if (stmt->assign != nullptr) {
        traverse_assign(std::move(stmt->assign));
    }
}

void TACGenerator::traverse_assign(AssignStmtAST* assign) {
    //*output << assign->varID->lexeme << '=';
    std::cout << "Entering Assignment...\n";
}