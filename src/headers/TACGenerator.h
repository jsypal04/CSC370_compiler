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

    void traverse_prog(std::unique_ptr<ProgStmtAST> prog);

    void traverse_stmt(std::unique_ptr<LineStmtAST> stmt);

    // this function may not be necessary
    void traverse_decl(std::unique_ptr<DeclarationStmtAST> decl);

    void traverse_assign(std::unique_ptr<AssignStmtAST> assign);

    void traverse_expr(std::unique_ptr<ExprStmtAST> expr);

    void traverse_expr_p(std::unique_ptr<Expr_PStmtAST> expr_p);

    void traverse_term(std::unique_ptr<TermStmtAST> term);

    void traverse_term_p(std::unique_ptr<Term_PStmtAST> term_p);

    void traverse_factor(std::unique_ptr<FactorStmtAST> factor);
};

#endif