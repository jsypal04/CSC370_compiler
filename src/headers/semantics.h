#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <unordered_map>
#include "ast.h"

// class to traverse the ast and do type checking
class Semantics {
private:
    std::unordered_map<std::string, Token> variables; // variable to map variable names to their type

public:
    void traverse_prog(std::unique_ptr<ProgStmtAST> prog);

    void traverse_stmt(std::unique_ptr<LineStmtAST> stmt);

    void traverse_decl(std::unique_ptr<DeclarationStmtAST> decl);

    void traverse_assign(std::unique_ptr<AssignStmtAST> assign);

    Token traverse_expr(std::unique_ptr<ExprStmtAST> expr); // returns the type of the expression

    Token traverse_expr_p(std::unique_ptr<Expr_PStmtAST> expr_p);

    Token traverse_term(std::unique_ptr<TermStmtAST> term);

    Token traverse_term_p(std::unique_ptr<Term_PStmtAST> term_p);

    Token traverse_factor(std::unique_ptr<FactorStmtAST> factor);

};

#endif