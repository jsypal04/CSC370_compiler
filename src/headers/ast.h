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
    std::unique_ptr<LineStmtAST> stmt;
    std::unique_ptr<ProgStmtAST> prog;

    ProgStmtAST(std::unique_ptr<LineStmtAST> first_stmt, std::unique_ptr<ProgStmtAST> program) {
        stmt = std::move(first_stmt);
        prog = std::move(program);
    }
};

// class for Stmts
class LineStmtAST : public StmtAST {
public:
    std::unique_ptr<DeclarationStmtAST> declaration;
    std::unique_ptr<AssignStmtAST> assign;

    LineStmtAST(std::unique_ptr<DeclarationStmtAST> decl_ptr, std::unique_ptr<AssignStmtAST> assign_ptr) {
        declaration = std::move(decl_ptr);
        assign = std::move(assign_ptr);
    }
};

// class for declaration nodes
class DeclarationStmtAST : public StmtAST {
public:
    std::unique_ptr<IDStmtAST> type;
    std::unique_ptr<IDStmtAST> variable;
    
    DeclarationStmtAST(std::unique_ptr<IDStmtAST> data_type, std::unique_ptr<IDStmtAST> varName) {
        type = std::move(data_type);
        variable = std::move(varName);
    }
};

// class for assignment nodes
class AssignStmtAST : public StmtAST {
public:
    std::unique_ptr<IDStmtAST> varID;
    std::unique_ptr<ExprStmtAST> RHS;

    AssignStmtAST(std::unique_ptr<IDStmtAST> name, std::unique_ptr<ExprStmtAST> rhs) {
        varID = std::move(name);
        RHS = std::move(rhs);
    }
};

// class for arithmetic expression nodes
class ExprStmtAST : public StmtAST {
public:
    std::unique_ptr<TermStmtAST> term;
    std::unique_ptr<Expr_PStmtAST> expr_p;

    ExprStmtAST(std::unique_ptr<TermStmtAST> trm, std::unique_ptr<Expr_PStmtAST> exp) {
        term = std::move(trm);
        expr_p = std::move(exp);
    }
};

// class for the expr_p production rule
class Expr_PStmtAST : public StmtAST {
public:
    Token op;
    std::unique_ptr<TermStmtAST> term;
    std::unique_ptr<Expr_PStmtAST> expr_p;

    Expr_PStmtAST(Token o, std::unique_ptr<TermStmtAST> trm, std::unique_ptr<Expr_PStmtAST> exp) {
        op = o;
        term = std::move(trm);
        expr_p = std::move(exp);
    }
};

class TermStmtAST : public StmtAST {
public:
    std::unique_ptr<FactorStmtAST> factor;
    std::unique_ptr<Term_PStmtAST> term_p;

    TermStmtAST(std::unique_ptr<FactorStmtAST> fctr, std::unique_ptr<Term_PStmtAST> trm_p) {
        factor = std::move(fctr);
        term_p = std::move(trm_p);
    }
};

class Term_PStmtAST : public StmtAST {
public:
    Token op;
    std::unique_ptr<FactorStmtAST> factor;
    std::unique_ptr<Term_PStmtAST> term_p;

    Term_PStmtAST(Token o, std::unique_ptr<FactorStmtAST> fctr, std::unique_ptr<Term_PStmtAST> trm_p) {
        op = o;
        factor = std::move(fctr);
        term_p = std::move(trm_p);
    }
};

class FactorStmtAST : public StmtAST {
public:
    std::unique_ptr<IDStmtAST> object;
    std::unique_ptr<ExprStmtAST> expr_object;

    // Enter (nullptr, ExprStmtAST) if the factor is an expression and (IDStmtAST, nullptr) if the factor is a variable/literal
    FactorStmtAST(std::unique_ptr<IDStmtAST> obj, std::unique_ptr<ExprStmtAST> expr_obj) {
        object = std::move(obj);
        expr_object = std::move(expr_obj);
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