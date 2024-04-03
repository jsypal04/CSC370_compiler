#ifndef AST_H
#define AST_H

#include <string>
#include <memory>
#include "tokens.h"

// class declarations
class StmtAST;
class ProgStmtAST;
class LineStmtAST;
class DeclarationStmtAST;
class AssignStmtAST;
class ExprStmtAST;
class Expr_PStmtAST;
class TermStmtAST;
class Term_PStmtAST;
class FactorStmtAST;
class IDStmtAST;

// base class for all statements
class StmtAST {
public:
    virtual ~StmtAST() = default;
};

// class for prog
class ProgStmtAST : public StmtAST {
public:
    LineStmtAST* stmt;
    ProgStmtAST* prog;

    ProgStmtAST(LineStmtAST* first_stmt, ProgStmtAST* program) {
        stmt = first_stmt;
        prog = program;
    }
};

// class for Stmts
class LineStmtAST : public StmtAST {
public:
    DeclarationStmtAST* declaration;
    AssignStmtAST* assign;

    LineStmtAST(DeclarationStmtAST* decl_ptr, AssignStmtAST* assign_ptr) {
        declaration = decl_ptr;
        assign = assign_ptr;
    }
};

// class for declaration nodes
class DeclarationStmtAST : public StmtAST {
public:
    IDStmtAST* type;
    IDStmtAST* variable;
    
    DeclarationStmtAST(IDStmtAST* data_type, IDStmtAST* varName) {
        type = data_type;
        variable = varName;
    }
};

// class for assignment nodes
class AssignStmtAST : public StmtAST {
public:
    IDStmtAST* varID;
    ExprStmtAST* RHS;

    AssignStmtAST(IDStmtAST* name, ExprStmtAST* rhs) {
        varID = name;
        RHS = rhs;
    }
};

// class for arithmetic expression nodes
class ExprStmtAST : public StmtAST {
public:
    TermStmtAST* term;
    Expr_PStmtAST* expr_p;

    ExprStmtAST(TermStmtAST* trm, Expr_PStmtAST* exp) {
        term = trm;
        expr_p = exp;
    }
};

// class for the expr_p production rule
class Expr_PStmtAST : public StmtAST {
public:
    Token op;
    TermStmtAST* term;
    Expr_PStmtAST* expr_p;

    Expr_PStmtAST(Token o, TermStmtAST* trm, Expr_PStmtAST* exp) {
        op = o;
        term = trm;
        expr_p = exp;
    }
};

class TermStmtAST : public StmtAST {
public:
    FactorStmtAST* factor;
    Term_PStmtAST* term_p;

    TermStmtAST(FactorStmtAST* fctr, Term_PStmtAST* trm_p) {
        factor = fctr;
        term_p = trm_p;
    }
};

class Term_PStmtAST : public StmtAST {
public:
    Token op;
    FactorStmtAST* factor;
    Term_PStmtAST* term_p;

    Term_PStmtAST(Token o, FactorStmtAST* fctr, Term_PStmtAST* trm_p) {
        op = o;
        factor = fctr;
        term_p = trm_p;
    }
};

class FactorStmtAST : public StmtAST {
public:
    IDStmtAST* object;
    ExprStmtAST* expr_object;

    // Enter (nullptr, ExprStmtAST) if the factor is an expression and (IDStmtAST, nullptr) if the factor is a variable/literal
    FactorStmtAST(IDStmtAST* obj, ExprStmtAST* expr_obj) {
        object = obj;
        expr_object = expr_obj;
    }
};

// class to represent identifiers and keywords
class IDStmtAST : public StmtAST {
public:
    Token token;
    std::string lexeme;

    IDStmtAST(Token tok, std::string lex) {
        token = tok;
        lexeme = lex;
    }
};

#endif