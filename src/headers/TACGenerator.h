#ifndef TACGENERATOR_H
#define TACGENERATOR_H

#include <fstream>
#include "ast.h"

// Class to traverse the AST and produce a three address code translation of the source code in a .tac file
class TACGenerator {
private:
    std::ofstream* output = new std::ofstream("a.tac");
    int tempCount = 0;

public:
    void destroyStream() {
        output->close();
        delete output;
    }

    void traverse_prog(ProgStmtAST* prog);

    void traverse_stmt(LineStmtAST* stmt);

    void traverse_assign(AssignStmtAST* assign);

    std::string traverse_expr(ExprStmtAST* expr);

    std::string traverse_expr_p(Expr_PStmtAST* expr_p);

    std::string traverse_term(TermStmtAST* term);

    std::string traverse_term_p(Term_PStmtAST* term_p);

    std::string traverse_factor(FactorStmtAST* factor);
};

#endif