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

// class for assignment nodes
class AssignStmtAST : StmtAST {
private:
    std::string varName;
    Token type;
    std::unique_ptr<StmtAST> RHS;
public:
    AssignStmtAST(std::string name, Token typ, std::unique_ptr<StmtAST> rhs) {
        varName = name;
        type = typ;
        RHS = std::move(rhs); // no idea why we need to use std::move() but it doesn't work otherwise
    }
};

// class for arithmetic expression nodes
class ExprStmtAST : StmtAST {
private:
    std::unique_ptr<StmtAST> term;
    std::unique_ptr<StmtAST> expr_p;
public:
    ExprStmtAST(std::unique_ptr<StmtAST> trm, std::unique_ptr<StmtAST> exp) {
        term = std::move(trm);
        expr_p = std::move(exp);
    }
};

// class for the expr_p production rule
class Expr_PStmtAST : StmtAST {
private:
    Token op;
    std::unique_ptr<StmtAST> term;
    std::unique_ptr<StmtAST> expr_p;
public:
    Expr_PStmtAST(Token o, std::unique_ptr<StmtAST> trm, std::unique_ptr<StmtAST> exp) {
        op = o;
        term = std::move(trm);
        expr_p = std::move(exp);
    }
};

class TermStmtAST : StmtAST {
private:
    std::unique_ptr<StmtAST> factor;
    std::unique_ptr<StmtAST> term_p;
public:
    TermStmtAST(std::unique_ptr<StmtAST> fctr, std::unique_ptr<StmtAST> trm_p) {
        factor = std::move(fctr);
        term_p = std::move(trm_p);
    }
};

class Term_PStmtAST : StmtAST {
private:
    Token op;
    std::unique_ptr<StmtAST> factor;
    std::unique_ptr<StmtAST> term_p;
public:
    Term_PStmtAST(Token o, std::unique_ptr<StmtAST> fctr, std::unique_ptr<StmtAST> trm_p) {
        op = o;
        factor = std::move(fctr);
        term_p = std::move(trm_p);
    }
};

class FactorStmtAST : StmtAST {
private:
    // might change how lparen and rparen are implemented
    bool lparen;
    std::unique_ptr<StmtAST> object;
    bool rparen;
public:
    // only input tokens for lparen and rparen if obj is an ExprStmtAST, othermise input null
    FactorStmtAST(bool lp, bool rp, std::unique_ptr<StmtAST> obj) {
        lparen = lp;
        rparen = rp;
        object = std::move(obj);
    }
};

// class to represent identifiers and keywords
class IDStmtAST : StmtAST {
private:
    Token token;
    std::string lexeme;
public:
    IDStmtAST(Token tok, std::string lex) {
        token = tok;
        lexeme = lex;
    }
};

// class to represent literals 
class LiteralStmtAST : StmtAST {
private:
    Token type;
    std::string value;
public:
    LiteralStmtAST(Token data_type, std::string val) {
        type = data_type;
        value = val;
    }
};

#endif