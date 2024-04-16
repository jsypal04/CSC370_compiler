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

    /******************************************************************
     * FUNCTION DEFINITIONS FOR ARITHMETIC EXPRESSION SEMANTIC ANALYSIS
    ******************************************************************/

    // Traverses the expression node
    Token traverse_expr(const ExprStmtAST* expr); // returns the type of the expression

    // Traverses the expression prime node
    Token traverse_expr_p(Expr_PStmtAST* expr_p);

    // Traverses the term node
    Token traverse_term(TermStmtAST* term);

    // Traverses the term prime node
    Token traverse_term_p(Term_PStmtAST* term_p);

    // Traverses the factor node
    Token traverse_factor(FactorStmtAST* factor);

    /***************************************************************
     * FUNCTION DEFINITIONS FOR BOOLEAN EXPRESSION SEMANTIC ANALYSIS
    ***************************************************************/

    // Traverses the bool_expr node
    Token traverse_bool_expr(const BoolExpr* expr);

    // Traverses the bool_term node
    Token traverse_bool_term(BoolTerm* term);

    // Traverses the bool_factor node
    Token traverse_bool_factor(BoolFactor* factor);

    // Traverses the super_rel node
    Token traverse_super_rel(const SuperRel* super_rel);

    // Traverses the relation node
    Token traverse_relation(Relation* rel);

    // Traverses the rel_operand node
    Token traverse_rel_operand(IDStmtAST* operand);
};

#endif