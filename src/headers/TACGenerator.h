#ifndef TACGENERATOR_H
#define TACGENERATOR_H

#include <fstream>
#include "ast.h"

// Class to traverse the AST and produce a three address code translation of the source code in a .tac file
class TACGenerator {
private:
    std::ofstream* output = new std::ofstream("a.tac");

public:
    void destroyStream() {
        output->close();
        delete output;
    }

    void traverse_prog(ProgStmtAST* prog);

    void traverse_stmt(LineStmtAST* stmt);

    // this function may not be necessary
    void traverse_decl(DeclarationStmtAST* decl);

    void traverse_assign(AssignStmtAST* assign);

    void traverse_expr(ExprStmtAST* expr);

    void traverse_expr_p(Expr_PStmtAST* expr_p);

    void traverse_term(TermStmtAST* term);

    void traverse_term_p(Term_PStmtAST* term_p);

    void traverse_factor(FactorStmtAST* factor);
};

#endif