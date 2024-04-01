#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <unordered_map>
#include "ast.h"

// class to traverse the ast and do type checking
class Semantics {
private:
    // Variable to map declared variable names to their type
    std::unordered_map<std::string, Token> variables;

public:
    // These are functions to traverse the AST and check for semantic errors

    // Traverses the program node
    std::unique_ptr<ProgStmtAST> traverse_prog(std::unique_ptr<ProgStmtAST> prog);

    // Traverses the statement node
    void traverse_stmt(std::unique_ptr<LineStmtAST> stmt);

    // Traverses the declaration node
    void traverse_decl(std::unique_ptr<DeclarationStmtAST> decl);

    // Traverses the assignment node
    void traverse_assign(std::unique_ptr<AssignStmtAST> assign);

    // Traverses the expression node
    Token traverse_expr(std::unique_ptr<ExprStmtAST> expr); // returns the type of the expression

    // Traverses the expression prime node
    Token traverse_expr_p(std::unique_ptr<Expr_PStmtAST> expr_p);

    // Traverses the term node
    Token traverse_term(std::unique_ptr<TermStmtAST> term);

    // Traverses the term prime node
    Token traverse_term_p(std::unique_ptr<Term_PStmtAST> term_p);

    // Traverses the factor node
    Token traverse_factor(std::unique_ptr<FactorStmtAST> factor);

};

#endif