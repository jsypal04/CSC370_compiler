#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <fstream>
#include <unordered_map>
#include <stack>
#include "ast.h"

// class to traverse the ast and do type checking
class Semantics {
private:
    // Variable to map declared variable names to their type
    std::unordered_map<std::string, Token> variables;

public:
    // These are functions to traverse the AST and check for semantic errors

    // Traverses the program node
    void traverse_prog(ProgStmtAST* prog);

    // Traverses the statement node
    void traverse_stmt(LineStmtAST* stmt);

    // Traverses the declaration node
    void traverse_decl(DeclarationStmtAST* decl);

    // Traverses the assignment node
    void traverse_assign(AssignStmtAST* assign);

    // Traverses the expression node
    Token traverse_expr(ExprStmtAST* expr); // returns the type of the expression

    // Traverses the expression prime node
    Token traverse_expr_p(Expr_PStmtAST* expr_p);

    // Traverses the term node
    Token traverse_term(TermStmtAST* term);

    // Traverses the term prime node
    Token traverse_term_p(Term_PStmtAST* term_p);

    // Traverses the factor node
    Token traverse_factor(FactorStmtAST* factor);

};

#endif