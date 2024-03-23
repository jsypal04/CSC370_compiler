#ifndef AST_H
#define AST_H

#include <string>
#include <memory>
#include "tokens.h"

// base class for all statements
class StmtAST {
public:
    virtual ~StmtAST() = default;
};

// class for prog
class ProgStmtAST : StmtAST {
public:
    std::unique_ptr<StmtAST> stmt;
    std::unique_ptr<StmtAST> prog;

    ProgStmtAST(std::unique_ptr<StmtAST> first_stmt, std::unique_ptr<StmtAST> program) {
        stmt = std::move(first_stmt);
        prog = std::move(program);
    }
};

// class for Stmts
class LineStmtAST : StmtAST {
public:
    std::unique_ptr<StmtAST> declaration;
    std::unique_ptr<StmtAST> assign;

    LineStmtAST(std::unique_ptr<StmtAST> decl_ptr, std::unique_ptr<StmtAST> assign_ptr) {
        declaration = std::move(decl_ptr);
        assign = std::move(assign_ptr);
    }
};

// class for declaration nodes
class DeclarationStmtAST : StmtAST {
public:
    std::unique_ptr<StmtAST> type;
    std::unique_ptr<StmtAST> variable;
    
    DeclarationStmtAST(std::unique_ptr<StmtAST> data_type, std::unique_ptr<StmtAST> varName) {
        type = std::move(data_type);
        variable = std::move(varName);
    }
};

// class for assignment nodes
class AssignStmtAST : StmtAST {
public:
    std::unique_ptr<StmtAST> varID;
    std::unique_ptr<StmtAST> RHS;

    AssignStmtAST(std::unique_ptr<StmtAST> name, std::unique_ptr<StmtAST> rhs) {
        varID = std::move(name);
        RHS = std::move(rhs);
    }
};

// class for arithmetic expression nodes
class ExprStmtAST : StmtAST {
public:
    std::unique_ptr<StmtAST> term;
    std::unique_ptr<StmtAST> expr_p;

    ExprStmtAST(std::unique_ptr<StmtAST> trm, std::unique_ptr<StmtAST> exp) {
        term = std::move(trm);
        expr_p = std::move(exp);
    }
};

// class for the expr_p production rule
class Expr_PStmtAST : StmtAST {
public:
    Token op;
    std::unique_ptr<StmtAST> term;
    std::unique_ptr<StmtAST> expr_p;

    Expr_PStmtAST(Token o, std::unique_ptr<StmtAST> trm, std::unique_ptr<StmtAST> exp) {
        op = o;
        term = std::move(trm);
        expr_p = std::move(exp);
    }
};

class TermStmtAST : StmtAST {
public:
    std::unique_ptr<StmtAST> factor;
    std::unique_ptr<StmtAST> term_p;

    TermStmtAST(std::unique_ptr<StmtAST> fctr, std::unique_ptr<StmtAST> trm_p) {
        factor = std::move(fctr);
        term_p = std::move(trm_p);
    }
};

class Term_PStmtAST : StmtAST {
public:
    Token op;
    std::unique_ptr<StmtAST> factor;
    std::unique_ptr<StmtAST> term_p;

    Term_PStmtAST(Token o, std::unique_ptr<StmtAST> fctr, std::unique_ptr<StmtAST> trm_p) {
        op = o;
        factor = std::move(fctr);
        term_p = std::move(trm_p);
    }
};

class FactorStmtAST : StmtAST {
public:
    // might change how lparen and rparen are implemented
    std::unique_ptr<StmtAST> object;

    // only input tokens for lparen and rparen if obj is an ExprStmtAST, othermise input null
    FactorStmtAST(std::unique_ptr<StmtAST> obj) {
        object = std::move(obj);
    }
};

// class to represent identifiers and keywords
class IDStmtAST : StmtAST {
public:
    Token token;
    std::string lexeme;

    IDStmtAST(Token tok, std::string lex) {
        token = tok;
        lexeme = lex;
    }
};

#endif